add_executable(test_commutativity test_commutativity.cpp)
target_link_libraries(test_commutativity crdt)
add_test(NAME commutativity COMMAND test_commutativity)

add_executable(test_idempotency test_idempotency.cpp)
target_link_libraries(test_idempotency crdt)
add_test(NAME idempotency COMMAND test_idempotency)

add_executable(test_out_of_order test_out_of_order.cpp)
target_link_libraries(test_out_of_order crdt)
add_test(NAME out_of_order COMMAND test_out_of_order)

add_executable(test_insert test_insert.cpp)
target_link_libraries(test_insert crdt)
add_test(NAME insert COMMAND test_insert)

add_executable(test_erase test_erase.cpp)
target_link_libraries(test_erase crdt)
add_test(NAME erase COMMAND test_erase)