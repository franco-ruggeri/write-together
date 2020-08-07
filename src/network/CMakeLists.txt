cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(QOBJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/cte/network)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

qt5_wrap_cpp(QOBJECT_SOURCES ${QOBJECT_INCLUDE_DIR}/TcpSocket.h)

add_library(network TcpSocket.cpp ${QOBJECT_SOURCES} ${QOBJECT_INCLUDE_DIR}/TcpSocket.h)

target_link_libraries(network protocol)

target_include_directories(network PUBLIC ${QOBJECT_INLCUDE_DIR})