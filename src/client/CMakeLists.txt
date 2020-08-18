set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/client)

find_package(Qt5 COMPONENTS Core Widgets Gui Network Concurrent PrintSupport REQUIRED)

list(APPEND SRC
        client.cpp
        loginTextEditor.cpp
        utility.cpp
        newFileDialog.cpp
        changePasswordDialog.cpp
        texteditor.cpp
        myClient.cpp
        fileInfo.cpp
        changeUsernameDialog.cpp
        UserInfo.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/changePasswordDialog.h
        ${QOBJ_DIR}/changeUsernameDialog.h
        ${QOBJ_DIR}/loginTextEditor.h
        ${QOBJ_DIR}/myClient.h
        ${QOBJ_DIR}/newFileDialog.h
        ${QOBJ_DIR}/texteditor.h
        ${QOBJ_DIR}/UserInfo.h)

list(APPEND LIB Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Concurrent Qt5::PrintSupport protocol crdt network)
list(APPEND INCLUDE ${QOBJ_DIR})

add_executable(client ${SRC} ${QOBJ_HEADERS} resource.qrc ../../include/cte/client/QTStylesheet.h QTStylesheet.cpp editProfileDialog.cpp)
target_link_libraries(client ${LIB})
target_include_directories(client PUBLIC ${INCLUDE})
install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)