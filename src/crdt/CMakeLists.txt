find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(crdt Symbol.cpp SharedEditor.cpp Lseq.cpp)

target_link_libraries(crdt Qt5::Core)