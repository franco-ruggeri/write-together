/*
 * Message types for the application protocol (see docs/protocol.md).
 */

#pragma once

namespace cte {
    enum class MessageType : int {
        error=0,

        // identity management
        signup=1,
        signup_ok=2,
        login=3,
        logout=4,
        profile=5,
        profile_ok=6,

        // document management
        documents=7,
        create=8,
        open=9,
        close=10,
        document=11,

        // document editing
        insert=12,
        erase=13,
        cursor=14,
        format=15
    };
}
