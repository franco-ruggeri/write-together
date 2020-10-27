set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/server)
set(DB_SRC_DIR ${PROJECT_SOURCE_DIR}/src/database)

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

list(APPEND SRC
        main.cpp
        server.cpp
        worker.cpp
        saver.cpp
        identity_manager.cpp
        document_manager.cpp
        open_document.cpp
        ${DB_SRC_DIR}/database_utility_secret.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/server.h
        ${QOBJ_DIR}/worker.h
        ${QOBJ_DIR}/saver.h)

list(APPEND LIB Qt5::Core Qt5::Network Qt5::Sql crdt protocol network database crypto)
list(APPEND INCLUDE ${QOBJ_DIR} ${DB_SRC_DIR})

add_executable(server ${SRC} ${QOBJ_HEADERS} resource.qrc)
target_link_libraries(server ${LIB})
target_include_directories(server PUBLIC ${INCLUDE})
install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
    set_property(TARGET server PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()