cmake_minimum_required(VERSION 3.15)
project(collaborative_text_editor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(${BOOST_ROOT})

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
