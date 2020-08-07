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
        ${DB_SRC_DIR}/db_utility_secret.cpp
        )

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/Server.h
        ${QOBJ_DIR}/Worker.h
        ${QOBJ_DIR}/DocumentManager.h
        ${QOBJ_DIR}/Saver.h
        )

list(APPEND LIB Qt5::Core Qt5::Network Qt5::Sql crdt protocol network database bcrypt)
list(APPEND INCLUDE ${QOBJ_DIR} ${DB_SRC_DIR})

add_executable(server ${SRC} ${QOBJ_HEADERS})
target_link_libraries(server ${LIB})
target_include_directories(server PUBLIC ${INCLUDE})
install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)