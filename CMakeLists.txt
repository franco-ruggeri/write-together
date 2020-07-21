cmake_minimum_required(VERSION 3.16)

project(collaborative_text_editor)

include_directories("${PROJECT_SOURCE_DIR}/include/protocol")
include_directories("${PROJECT_SOURCE_DIR}/include/crdt")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)