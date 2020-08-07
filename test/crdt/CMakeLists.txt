add_executable(test_commutativity test_commutativity.cpp)
target_link_libraries(test_commutativity crdt)
add_test(NAME commutativity COMMAND test_commutativity)

add_executable(test_idempotency test_idempotency.cpp)
target_link_libraries(test_idempotency crdt)
add_test(NAME idempotency COMMAND test_idempotency)

add_executable(test_insert test_insert.cpp)
target_link_libraries(test_insert crdt)
add_test(NAME insert COMMAND test_insert)