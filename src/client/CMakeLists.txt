cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(client client.cpp)

target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT} crdt)

install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)