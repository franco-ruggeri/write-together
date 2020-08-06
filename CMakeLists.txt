cmake_minimum_required(VERSION 3.15)
project(collaborative_text_editor)

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
