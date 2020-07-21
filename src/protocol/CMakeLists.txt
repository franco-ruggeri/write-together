cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5Core REQUIRED)

add_library(protocol
        Message.cpp
        ErrorMessage.cpp
        SignUpMessage.cpp
        LoginMessage.cpp
        LogoutMessage.cpp
        DocumentsMessage.cpp
        CreateMessage.cpp
        OpenMessage.cpp
        TextMessage.cpp
        InsertMessage.cpp
        EraseMessage.cpp
        CursorMessage.cpp
        CloseMessage.cpp
        )

target_link_libraries(protocol Qt5::Core)