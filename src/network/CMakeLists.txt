cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

add_library(network TcpSocket.cpp)

target_link_libraries(network protocol)