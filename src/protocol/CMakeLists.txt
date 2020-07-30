cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_library(protocol
        Message.cpp
        ErrorMessage.cpp
        SignupMessage.cpp
        LoginMessage.cpp
        LogoutMessage.cpp
        DocumentsMessage.cpp
        CreateMessage.cpp
        OpenMessage.cpp
        DocumentMessage.cpp
        InsertMessage.cpp
        EraseMessage.cpp
        CursorMessage.cpp
        CloseMessage.cpp
        ProfileMessage.cpp
        ProfileOkMessage.cpp
        Profile.cpp
        Document.cpp
        )

target_link_libraries(protocol crdt Qt5::Core Qt5::Gui)