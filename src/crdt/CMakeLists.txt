set(CMAKE_AUTOMOC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/crdt)

find_package(Qt5 COMPONENTS Core REQUIRED)

list(APPEND SRC shared_editor.cpp symbol.cpp lseq.cpp)
list(APPEND LIB Qt5::Core)
list(APPEND INCLUDE ${QOBJ_DIR})
list(APPEND QOBJ_HEADERS ${QOBJ_DIR}/shared_editor.h)

add_library(crdt STATIC ${SRC} ${QOBJ_HEADERS})
target_link_libraries(crdt ${LIB})
target_include_directories(crdt PUBLIC ${INCLUDE})