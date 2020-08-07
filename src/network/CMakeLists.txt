set(CMAKE_AUTOMOC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/network)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

list(APPEND SRC TcpSocket.cpp)
list(APPEND LIB Qt5::Core Qt5::Network protocol)
list(APPEND INCLUDE ${QOBJ_DIR})
list(APPEND QOBJ_HEADERS ${QOBJ_DIR}/TcpSocket.h)

add_library(network STATIC ${SRC} ${QOBJ_HEADERS})
target_link_libraries(network ${LIB})
target_include_directories(network PUBLIC ${INCLUDE})