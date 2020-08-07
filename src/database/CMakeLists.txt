cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Sql REQUIRED)

add_library(database DatabaseGuard.cpp db_utility.cpp)

target_link_libraries(database Qt5::Core Qt5::Sql)
target_include_directories(database PUBLIC include)