find_package(Qt5 COMPONENTS Core REQUIRED)

list(APPEND SRC symbol.cpp shared_editor.cpp lseq.cpp)
list(APPEND LIB Qt5::Core)
list(APPEND INCLUDE)

add_library(crdt STATIC ${SRC})
target_link_libraries(crdt ${LIB})
target_include_directories(crdt PUBLIC ${INCLUDE})