cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(crdt Symbol.cpp SharedEditor.cpp Lseq.cpp)

target_link_libraries(crdt Qt5::Core)