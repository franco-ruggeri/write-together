cmake_minimum_required(VERSION 3.15)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017")

set(CMAKE_CXX_STANDARD 20)


find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

add_library(protocol
        Message.cpp
        ErrorMessage.cpp
        SignUpMessage.cpp
        LoginMessage.cpp
        LogoutMessage.cpp
        DocumentsMessage.cpp
        CreateMessage.cpp
        OpenMessage.cpp
        InsertMessage.cpp
        EraseMessage.cpp
        CursorMessage.cpp
        CloseMessage.cpp
        ProfileMessage.cpp
        ProfileOkMessage.cpp
        Profile.cpp
        Document.cpp
        DocumentMessage.cpp
        )

target_link_libraries(protocol crdt Qt5::Core Qt5::Gui )