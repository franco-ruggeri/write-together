cmake_minimum_required(VERSION 3.15)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017")
project(collaborative_text_editor)

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)