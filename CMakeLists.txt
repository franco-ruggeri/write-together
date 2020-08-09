cmake_minimum_required(VERSION 3.15)
project(collaborative_text_editor)


#set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017")
include_directories("C:\\Program Files (x86)\\boost_1_73_0")
set(CMAKE_MSVC_RUNTIME_LIBRARY "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(lib/bcrypt/include)

add_subdirectory(src)
add_subdirectory(lib)

enable_testing()
add_subdirectory(test)
