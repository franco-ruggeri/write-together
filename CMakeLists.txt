cmake_minimum_required(VERSION 3.15)
project(collaborative_text_editor)
include_directories("C:\\Program Files (x86)\\boost_1_73_0")
include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)