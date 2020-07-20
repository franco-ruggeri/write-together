/*
 * Message types for the collaborative text editor application protocol.
 *
 * Author: Franco Ruggeri
 */

#pragma once

namespace collaborative_text_editor {
    enum class MessageType : int {
        insert=0,
        erase=1
    };
}
