cmake_minimum_required(VERSION 3.15)
#project(collaborative_text_editor)

set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017")

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5PrintSupport REQUIRED)



set(SOURCE_FILES client.cpp)

add_executable(collaborative_text_editor
        client.cpp
        loginTextEditor.cpp
        utility.cpp
        newFileDialog.cpp
        changePasswordDialog.cpp
        texteditor.cpp
        resource.qrc
        ../../include/client/loginTextEditor.h
        ../../include/client/newFileDialog.h
        ../../include/client/changePasswordDialog.h
        ../../include/client/utility.h
        ../../include/client/texteditor.h
        myClient.cpp
        ../../include/client/myClient.h
        ../../include/client/fileInfo.h
        ../protocol/Message.cpp
        ../protocol/LoginMessage.cpp
        ../protocol/SignUpMessage.cpp
        ../protocol/LogoutMessage.cpp
        ../protocol/InsertMessage.cpp
        ../protocol/CreateMessage.cpp
        ../protocol/EraseMessage.cpp
        ../protocol/ErrorMessage.cpp
        ../protocol/TextMessage.cpp
        ../protocol/CursorMessage.cpp
        ../protocol/CloseMessage.cpp
        ../protocol/ProfileMessage.cpp
        ../protocol/ProfileOkMessage.cpp
        ../protocol/OpenMessage.cpp
        ../protocol/DocumentsMessage.cpp
        ../crdt/SharedEditor.cpp
        ../crdt/Symbol.cpp
        ../crdt/LSEQ.cpp
        fileInfo.cpp
        ../protocol/User.cpp
        ../../include/protocol/User.h

        )

target_link_libraries(collaborative_text_editor Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::Concurrent Qt5::PrintSupport)

