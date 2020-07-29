cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(server main.cpp Server.cpp Worker.cpp User.cpp SafeSharedEditor.cpp)

target_link_libraries(server crdt protocol network Qt5::Core Qt5::Network)

install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)