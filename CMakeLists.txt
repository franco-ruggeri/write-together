cmake_minimum_required(VERSION 3.16)

project(collaborative_text_editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(lib/bcrypt/include)

add_subdirectory(src)
add_subdirectory(lib)

enable_testing()
add_subdirectory(test)