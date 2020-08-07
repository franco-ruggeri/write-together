list(APPEND SRC client.cpp)
list(APPEND LIB)
list(APPEND INCLUDE)

add_executable(client ${SRC})
target_link_libraries(client ${LIB})
target_include_directories(client PUBLIC ${INCLUDE})
install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)