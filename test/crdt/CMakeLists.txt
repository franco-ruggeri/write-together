cmake_minimum_required(VERSION 3.16)

add_executable(test_commutativity test_commutativity.cpp)
target_link_libraries(test_commutativity crdt)
add_test(NAME commutativity COMMAND test_commutativity CAT 1 H)
set_tests_properties(commutativity PROPERTIES PASS_REGULAR_EXPRESSION "^CAT\nCAT\nCHT\nCHT\n$")

add_executable(test_idempotency test_idempotency.cpp)
target_link_libraries(test_idempotency crdt)
add_test(NAME idempotency COMMAND test_idempotency HAT 0)
set_tests_properties(idempotency PROPERTIES PASS_REGULAR_EXPRESSION "^HAT\nHAT\nAT\nAT\n$")

add_executable(test_insert test_insert.cpp)
target_link_libraries(test_insert crdt)
add_test(NAME insert COMMAND test_insert)
