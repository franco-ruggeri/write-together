add_executable(client client.cpp)

target_link_libraries(client)

install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/bin)