cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(DATABASE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/database)
set(QOBJECT_INCLUDE_DIR include)

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

add_executable(server main.cpp Server.cpp Worker.cpp IdentityManager.cpp DocumentManager.cpp OpenDocument.cpp Saver.cpp
        ${DATABASE_SOURCE_DIR}/db_utility_secret.cpp ${QOBJECT_INCLUDE_DIR}/Server.h ${QOBJECT_INCLUDE_DIR}/Worker.h
        ${QOBJECT_INCLUDE_DIR}/DocumentManager.h ${QOBJECT_INCLUDE_DIR}/Saver.h)

target_link_libraries(server crdt protocol network database bcrypt Qt5::Core Qt5::Network Qt5::Sql)

target_include_directories(server PUBLIC ${QOBJECT_INCLUDE_DIR} ${DATABASE_SOURCE_DIR}/include)

install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)