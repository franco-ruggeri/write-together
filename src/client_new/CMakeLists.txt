set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QOBJ_DIR ${PROJECT_SOURCE_DIR}/include/cte/client_new)

find_package(Qt5 COMPONENTS Core REQUIRED)

list(APPEND SRC
        main.cpp
        client.cpp
        ui_worker.cpp
        network_worker.cpp)

list(APPEND QOBJ_HEADERS
        ${QOBJ_DIR}/client.h
        ${QOBJ_DIR}/ui_worker.h
        ${QOBJ_DIR}/network_worker.h)

list(APPEND LIB Qt5::Core network protocol widget)
list(APPEND INCLUDE ${QOBJ_DIR} ${UI_DIR})

add_executable(client ${SRC} ${QOBJ_HEADERS})
target_link_libraries(client ${LIB})
target_include_directories(client PUBLIC ${INCLUDE})
install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)