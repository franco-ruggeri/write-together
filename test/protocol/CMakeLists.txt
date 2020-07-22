cmake_minimum_required(VERSION 3.16)

add_executable(test_serialization_error test_serialization_error.cpp)
target_link_libraries(test_serialization_error protocol)
add_test(NAME serialization_error COMMAND test_serialization_error "test error")
set_tests_properties(serialization_error PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"reason\":\"test error\",\"type\":0}\n$"
        )

add_executable(test_serialization_sign_up test_serialization_sign_up.cpp)
target_link_libraries(test_serialization_sign_up protocol)
add_test(NAME serialization_sign_up COMMAND test_serialization_sign_up "test username" "test password")
set_tests_properties(serialization_sign_up PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"password\":\"test password\",\"type\":1,\"username\":\"test username\"}\n$"
        )

add_executable(test_serialization_login test_serialization_login.cpp)
target_link_libraries(test_serialization_login protocol)
add_test(NAME serialization_login COMMAND test_serialization_login "test username" "test password")
set_tests_properties(serialization_login PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"password\":\"test password\",\"type\":2,\"username\":\"test username\"}\n$"
        )

add_executable(test_serialization_logout test_serialization_logout.cpp)
target_link_libraries(test_serialization_logout protocol)
add_test(NAME serialization_logout COMMAND test_serialization_logout)
set_tests_properties(serialization_logout PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"type\":3}\n$"
        )

add_executable(test_serialization_documents test_serialization_documents.cpp)
target_link_libraries(test_serialization_documents protocol)
add_test(NAME serialization_documents COMMAND test_serialization_documents "test document 1" "test document 2")
set_tests_properties(serialization_documents PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"documents\":\\[\"test document 1\",\"test document 2\"\\],\"type\":4}\n$"
        )

add_executable(test_serialization_create test_serialization_create.cpp)
target_link_libraries(test_serialization_create protocol)
add_test(NAME serialization_create COMMAND test_serialization_create "test document")
set_tests_properties(serialization_create PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"type\":5}\n$"
        )

add_executable(test_serialization_open test_serialization_open.cpp)
target_link_libraries(test_serialization_open protocol)
add_test(NAME serialization_open COMMAND test_serialization_open "test document")
set_tests_properties(serialization_open PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"type\":6}\n$"
        )

add_executable(test_serialization_text test_serialization_text.cpp)
target_link_libraries(test_serialization_text protocol)
add_test(NAME serialization_text COMMAND test_serialization_text "test document" "test document text")
set_tests_properties(serialization_text PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"text\":\"test document text\",\"type\":7}\n$"
        )

add_executable(test_serialization_insert test_serialization_insert.cpp)
target_link_libraries(test_serialization_insert protocol crdt)
add_test(NAME serialization_insert COMMAND test_serialization_insert "test document" "C" 1 5 2 4 5)
set_tests_properties(serialization_insert PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"symbol\":{\"position\":\\[2,4,5\\],\"site_counter\":5,\"site_id\":1,\"value\":\"C\"},\"type\":8}\n$"
        )

add_executable(test_serialization_erase test_serialization_erase.cpp)
target_link_libraries(test_serialization_erase protocol crdt)
add_test(NAME serialization_erase COMMAND test_serialization_erase "test document" "C" 1 5 2 4 5)
set_tests_properties(serialization_erase PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"symbol\":{\"position\":\\[2,4,5\\],\"site_counter\":5,\"site_id\":1,\"value\":\"C\"},\"type\":9}\n$"
        )

add_executable(test_serialization_cursor test_serialization_cursor.cpp)
target_link_libraries(test_serialization_cursor protocol crdt)
add_test(NAME serialization_cursor COMMAND test_serialization_cursor "test document" "test username" "C" 1 5 2 4 5)
set_tests_properties(serialization_cursor PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"symbol\":{\"position\":\\[2,4,5\\],\"site_counter\":5,\"site_id\":1,\"value\":\"C\"},\"type\":10,\"username\":\"test username\"}\n$"
        )

add_executable(test_serialization_close test_serialization_close.cpp)
target_link_libraries(test_serialization_close protocol)
add_test(NAME serialization_close COMMAND test_serialization_close "test document" "test username")
set_tests_properties(serialization_close PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"document\":\"test document\",\"type\":11,\"username\":\"test username\"}\n$"
        )

add_executable(test_serialization_profile test_serialization_profile.cpp)
target_link_libraries(test_serialization_profile protocol)
add_test(NAME serialization_profile COMMAND test_serialization_profile "test username" "test password" 1 1 0 0 255)
set_tests_properties(serialization_profile PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"icon\":\"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAACXBIWXMAAA9hAAAPYQGoP6dpAAAADElEQVQImWNgYPgPAAEDAQBdlO9aAAAAAElFTkSuQmCC\",\"password\":\"test password\",\"type\":12,\"username\":\"test username\"}\n$"
        )

add_executable(test_serialization_profile_ok test_serialization_profile_ok.cpp)
target_link_libraries(test_serialization_profile_ok protocol)
add_test(NAME serialization_profile_ok COMMAND test_serialization_profile_ok)
set_tests_properties(serialization_profile_ok PROPERTIES PASS_REGULAR_EXPRESSION
        "^{\"type\":13}\n$"
        )