/*
 * Message types for the collaborative text editor application protocol (see docs/protocol.md).
 *
 * Author: Franco Ruggeri
 */

#pragma once

namespace editor {
    enum class MessageType : int {
        error=0,

        // identity management
        signup=1,
        login=2,
        logout=3,
        profile=4,
        profile_ok=5,

        // document management
        documents=6,
        create=7,
        open=8,
        close=9,
        document=10,

        // document editing
        insert=11,
        erase=12,
        cursor=13
    };
}
