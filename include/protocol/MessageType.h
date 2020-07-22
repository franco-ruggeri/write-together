/*
 * Message types for the collaborative text editor application protocol.
 *
 * Author: Franco Ruggeri
 */

#pragma once

namespace collaborative_text_editor {
    enum class MessageType : int {
        error=0,
        sign_up=1,
        login=2,
        logout=3,
        documents=4,
        create=5,
        open=6,
        text=7,
        insert=8,
        erase=9,
        cursor=10,
        close=11,
        profile=12,
        profile_ok=13
    };
}
