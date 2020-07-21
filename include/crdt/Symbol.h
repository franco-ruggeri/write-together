/*
 * Encapsulation of a single character in the text editor with all the information needed for CRDT.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>

namespace collaborative_text_editor {
    class Symbol {
        char value_;
        int site_id_, site_counter_;
        std::vector<int> position_;

    public:
        Symbol(char value, int site_id, int site_counter, const std::vector<int>& position);

        bool operator<(const Symbol& other);
        bool operator==(const Symbol& other);

        char value() const;
        int site_id() const;
        int site_counter() const;
        std::vector<int> position() const;
    };
}
