#include <cte/crdt/lseq.h>
#include <limits>
#include <stdexcept>

namespace cte {
    // max range divided by 4 to avoid overflows in between()
    const int Lseq::begin = std::numeric_limits<int>::min() / 4 + 1;    // +1 to have begin_ = -end_ (easier to debug)
    const int Lseq::end = std::numeric_limits<int>::max() / 4;
    const unsigned int Lseq::default_boundary = 100;

    Lseq::Lseq(unsigned int boundary) : boundary_(boundary) {}

    QVector<int> Lseq::between(QVector<int> prev, QVector<int> next) {
        // fill with begin_ (e.g. begin_=0 => 1 == 1.0 == 1.00)
        auto prev_depth = prev.size();
        auto next_depth = next.size();
        auto max_depth = std::max(prev_depth, next_depth) + 1;  // +1 to have for sure the last level with available pos
        std::fill_n(std::back_inserter(prev), max_depth-prev_depth, begin);
        std::fill_n(std::back_inserter(next), max_depth-next_depth, begin);

        // find interval for allocation
        int depth=-1, interval=0;
        bool consecutive = false;
        while (interval < 1) {
            depth++;
            if (!consecutive) {
                // no consecutive nodes at a previous level => the interval is straightforward
                // example: prev=1, next=5 => interval = 5-1-1 = 3 (i.e. positions 2, 3, 4)
                interval = next[depth] - prev[depth] - 1;
                if (interval == 0)
                    consecutive = true;
            } else {
                // consecutive nodes at a previous level => the interval is made of two parts
                // example: prev=1.5, next=2.2, begin_=0, end_=9 => interval = 9-5-1 + 2-0-1 = 4
                // (i.e. positions 1.6, 1.7, 1.8, 2.1)
                interval = 0;
                if (next[depth] != begin) interval += next[depth] - begin - 1;
                if (prev[depth] != end) interval += end - prev[depth] - 1;
            }
        }
        interval = std::min<int>(boundary_, interval);

        // set strategy
        Strategy strategy;
        if (depth < strategies_.size())
            strategy = strategies_[depth];
        else {
            // randomly choose
            strategy = (std::rand() & 1) ? Strategy::boundary_plus : Strategy::boundary_minus;
            strategies_.push_back(strategy);
        }

        // allocate
        QVector<int> between;
        int step = rand() % interval + 1;
        switch (strategy) {
            case Strategy::boundary_plus:
                // if we enter in this if, it means that at a previous level we had consecutive nodes
                // it may be that the step requires to jump to a child node of next, instead of prev
                // example: prev=1.5, next=2.5, begin_=0, end=9, step=5 => leaf = 5-(9-5-1) = 2 (i.e. position 2.2)
                if (prev[depth] + step >= end) {
                    std::copy(next.begin(), next.begin()+depth, std::back_inserter(between));
                    between.push_back(begin + step - (prev[depth] == end ? 0 : end-prev[depth]-1));
                } else {
                    std::copy(prev.begin(), prev.begin()+depth, std::back_inserter(between));
                    between.push_back(prev[depth] + step);
                }
                break;
            case Strategy::boundary_minus:
                // same as above, but reversed
                if (next[depth] - step <= begin) {
                    std::copy(prev.begin(), prev.begin()+depth, std::back_inserter(between));
                    between.push_back(end - step + (next[depth] == begin ? 0 : next[depth]-begin-1));
                } else {
                    std::copy(next.begin(), next.begin()+depth, std::back_inserter(between));
                    between.push_back(next[depth] - step);
                }
                break;
            default:
                throw std::logic_error("invalid Strategy");
        }

        return between;
    }
}
