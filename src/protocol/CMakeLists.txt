find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_library(protocol
        Message.cpp
        Profile.cpp
        Document.cpp
        DocumentData.cpp
        ErrorMessage.cpp
        SignupMessage.cpp
        SignupOkMessage.cpp
        LoginMessage.cpp
        LogoutMessage.cpp
        ProfileMessage.cpp
        ProfileOkMessage.cpp
        DocumentsMessage.cpp
        CreateMessage.cpp
        OpenMessage.cpp
        CloseMessage.cpp
        DocumentMessage.cpp
        InsertMessage.cpp
        EraseMessage.cpp
        CursorMessage.cpp
        )

target_link_libraries(protocol crdt Qt5::Core Qt5::Gui)