#include <cte/protocol/format.h>

namespace cte {
    Format::Format() : bold_(false), italic_(false), underlined_(false) {}

    Format::Format(bool bold, bool italic, bool underlined) : bold_(bold), italic_(italic), underlined_(underlined) {}

    Format::Format(const QJsonObject &json_object) {
        auto end_iterator = json_object.end();
        auto bold_iterator = json_object.find("bold");
        auto italic_iterator = json_object.find("italic");
        auto underlined_iterator = json_object.find("underlined");

        if (bold_iterator == end_iterator || italic_iterator == end_iterator || underlined_iterator == end_iterator ||
            !bold_iterator->isBool() || !italic_iterator->isBool() || !underlined_iterator->isBool())
            throw std::logic_error("invalid message: invalid fields");

        bold_ = bold_iterator->toBool();
        italic_ = italic_iterator->toBool();
        underlined_ = underlined_iterator->toBool();
    }

    bool Format::operator==(const Format& other) const {
        return this->bold_ == other.bold_ && this->italic_ == other.italic_ && this->underlined_ == other.underlined_;
    }

    QJsonObject Format::json() const {
        QJsonObject json_object;
        json_object["bold"] = bold_;
        json_object["italic"] = italic_;
        json_object["underlined"] = underlined_;
        return json_object;
    }

    bool Format::bold() const {
        return bold_;
    }

    bool Format::italic() const {
        return italic_;
    }

    bool Format::underlined() const {
        return underlined_;
    }
}
