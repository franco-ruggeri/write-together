cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

include_directories(..)

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

add_executable(server main.cpp Server.cpp Worker.cpp IdentityManager.cpp DocumentManager.cpp ../database/db_utility_secret.cpp)

target_link_libraries(server crdt protocol network database bcrypt Qt5::Core Qt5::Network Qt5::Sql)

install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)