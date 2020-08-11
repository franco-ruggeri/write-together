set(CMAKE_AUTOMOC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/server)
set(DB_SRC_DIR ${PROJECT_SOURCE_DIR}/src/database)

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

list(APPEND SRC
        main.cpp
        Server.cpp
        Worker.cpp
        IdentityManager.cpp
        DocumentManager.cpp
        OpenDocument.cpp
        Saver.cpp
        PasswordManager.cpp
        ${DB_SRC_DIR}/db_utility_secret.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/Server.h
        ${QOBJ_DIR}/Worker.h
        ${QOBJ_DIR}/Saver.h)


find_path(CRYPTOPP_H cryptlib.h REQUIRED)
find_library(CRYPTOPP_LIB cryptlib.lib REQUIRED)

list(APPEND LIB Qt5::Core Qt5::Network Qt5::Sql crdt protocol network database bcrypt ${CRYPTOPP_LIB})
list(APPEND INCLUDE ${QOBJ_DIR} ${DB_SRC_DIR} ${CRYPTOPP_H})

add_executable(server ${SRC} ${QOBJ_HEADERS})
if(WIN32)
    set_property(TARGET server PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()
target_link_libraries(server ${LIB})
target_include_directories(server PUBLIC ${INCLUDE})
install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)