cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017")
find_package(Qt5Core REQUIRED)

add_library(crdt Symbol.cpp SharedEditor.cpp Lseq.cpp)

target_link_libraries(crdt Qt5::Core)