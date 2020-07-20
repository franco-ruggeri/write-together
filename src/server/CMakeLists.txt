cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(server server.cpp)

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} crdt)

install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)