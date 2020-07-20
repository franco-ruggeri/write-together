/*
 * Instance of editor shared with other users through the network.
 *
 * Author: Franco Ruggeri
 */

#pragma once

#include <vector>
#include <string>
//#include <QTcpSocket>     // TODO: add in CMakeLists.txt
#include "Message.h"
#include "Symbol.h"
#include "LSEQ.h"

namespace collaborative_text_editor {
    class SharedEditor {
//        QTcpSocket server_socket_;
        int site_id_, site_counter_;
        std::vector<Symbol> text_;
        LSEQ pos_allocator_;

        void remote_insert(const Message& message);
        void remote_erase(const Message& message);

    public:
//        SharedEditor(QTcpSocket server_socket, int site_id);
        void local_insert(int index, char value);
        void local_erase(int index);
        void process(const Message& message);
        std::string to_string();
    };
}
