cmake_minimum_required(VERSION 3.15)

add_executable(test_commutativity test_commutativity.cpp)
target_link_libraries(test_commutativity crdt)
add_test(NAME commutativity COMMAND test_commutativity CAT 1 H)

add_executable(test_idempotency test_idempotency.cpp)
target_link_libraries(test_idempotency crdt)
add_test(NAME idempotency COMMAND test_idempotency HAT 0)

add_executable(test_insert test_insert.cpp)
target_link_libraries(test_insert crdt)
add_test(NAME insert COMMAND test_insert)
