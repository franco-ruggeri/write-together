set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/client_new)
set(UI_DIR ui)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

list(APPEND SRC
        main.cpp
        client.cpp
        worker_ui.cpp
        worker_network.cpp
        connection_form.cpp
        login_form.cpp
        signup_form.cpp
        profile_dialog.cpp
        home.cpp
        editor.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/client.h
        ${QOBJ_DIR}/worker_ui.h
        ${QOBJ_DIR}/worker_network.h
        ${QOBJ_DIR}/connection_form.h
        ${QOBJ_DIR}/login_form.h
        ${QOBJ_DIR}/signup_form.h
        ${QOBJ_DIR}/profile_dialog.h
        ${QOBJ_DIR}/home.h
        ${QOBJ_DIR}/editor.h)

list(APPEND LIB Qt5::Core Qt5::Widgets Qt5::PrintSupport network protocol crdt)
list(APPEND INCLUDE ${QOBJ_DIR} ${UI_DIR})

add_executable(client ${SRC} ${QOBJ_HEADERS} ${UI_DIR}/resource.qrc)
target_link_libraries(client ${LIB})
target_include_directories(client PUBLIC ${INCLUDE})
install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)