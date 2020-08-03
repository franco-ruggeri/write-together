cmake_minimum_required(VERSION 3.16)

project(collaborative_text_editor)

include_directories(include)
include_directories(lib/libbcrypt/include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)