/*
 * Adaptive allocator of variable-size identifiers (i.e. positions) of characters. The goal is to allocate smartly in
 * order to limit the identifier size.
 *
 * Two strategies are combined to adapt the allocation to the user's editing behavior:
 * - Boundary+, suitable for end editing.
 * - Boundary-, suitable for front editing.
 *
 * This is a simplified version of what proposed in "LSEQ: an Adaptive Structure for Sequences in
 * Distributed Collaborative Editing" (see docs/LSEQ.pdf). In particular, the base doubling is ignored because,
 * to get the advantages, it requires to work at bit-level. Since we represent for simplicity a node with an integer,
 * each node takes always 32 bits independently of the depth, so it would be a waste to use just a part of the
 * available bits.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>

namespace collaborative_text_editor {
    class LSEQ {
    public:
        enum class Strategy : bool {
            boundary_plus=true, boundary_minus=false
        };

    private:
        std::vector<Strategy> strategies_;      // strategy at each depth
        const unsigned int boundary_;

        static const int begin_;                // first possible child of a node, reserved
        static const int end_;                  // last possible child of a node, reserved
        static const unsigned int default_boundary;

    public:
        LSEQ();
        LSEQ(unsigned int boundary);
        static std::vector<int> begin();
        static std::vector<int> end();
        std::vector<int> between(std::vector<int> prev, std::vector<int> next);
        std::vector<Strategy> strategies() const;
    };
}
