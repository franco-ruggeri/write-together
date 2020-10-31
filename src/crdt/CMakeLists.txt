find_package(Qt5 COMPONENTS Core REQUIRED)

list(APPEND SRC shared_editor.cpp symbol.cpp lseq.cpp)
list(APPEND LIB Qt5::Core)

add_library(crdt STATIC ${SRC} ${QOBJ_HEADERS})
target_link_libraries(crdt ${LIB})
target_include_directories(crdt PUBLIC ${INCLUDE})