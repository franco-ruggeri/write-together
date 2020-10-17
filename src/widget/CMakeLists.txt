set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/widget)
set(UI_DIR ui)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

list(APPEND SRC
        widget.cpp
        form.cpp
        connect_form.cpp
        login_form.cpp
        signup_form.cpp
        home.cpp
        editor.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/widget.h
        ${QOBJ_DIR}/form.h
        ${QOBJ_DIR}/connect_form.h
        ${QOBJ_DIR}/login_form.h
        ${QOBJ_DIR}/signup_form.h
        ${QOBJ_DIR}/home.h
        ${QOBJ_DIR}/editor.h)

list(APPEND LIB Qt5::Core Qt5::Widgets crdt protocol)
list(APPEND INCLUDE ${QOBJ_DIR} ${UI_DIR})

add_library(widget STATIC ${SRC} ${QOBJ_HEADERS} ui/resource.qrc)
target_link_libraries(widget ${LIB})
target_include_directories(widget PUBLIC ${INCLUDE})