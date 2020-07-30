cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_serialization_error test_serialization_error.cpp)
target_link_libraries(test_serialization_error protocol)
add_test(NAME serialization_error COMMAND test_serialization_error "test error")

add_executable(test_serialization_sign_up test_serialization_sign_up.cpp)
target_link_libraries(test_serialization_sign_up protocol)
add_test(NAME serialization_sign_up COMMAND test_serialization_sign_up "test username" "test password")

add_executable(test_serialization_login test_serialization_login.cpp)
target_link_libraries(test_serialization_login protocol)
add_test(NAME serialization_login COMMAND test_serialization_login "test username" "test password")

add_executable(test_serialization_logout test_serialization_logout.cpp)
target_link_libraries(test_serialization_logout protocol)
add_test(NAME serialization_logout COMMAND test_serialization_logout)

add_executable(test_serialization_documents test_serialization_documents.cpp)
target_link_libraries(test_serialization_documents protocol)
add_test(NAME serialization_documents COMMAND test_serialization_documents "test owner 1" "test document 1" "test owner 2" "test document 2")

add_executable(test_serialization_create test_serialization_create.cpp)
target_link_libraries(test_serialization_create protocol)
add_test(NAME serialization_create COMMAND test_serialization_create "test document")

add_executable(test_serialization_open test_serialization_open.cpp)
target_link_libraries(test_serialization_open protocol)
add_test(NAME serialization_open COMMAND test_serialization_open "test owner" "test document" "fnsc:testuri")

add_executable(test_serialization_document test_serialization_document.cpp)
target_link_libraries(test_serialization_document protocol)
add_test(NAME serialization_document COMMAND test_serialization_document "test owner" "test document" "test text" "fnsc:testuri" 1 "test username" "test name" "test surname")

add_executable(test_serialization_insert test_serialization_insert.cpp)
target_link_libraries(test_serialization_insert protocol crdt)
add_test(NAME serialization_insert COMMAND test_serialization_insert "test document" "C" 1 5 2 4 5)

add_executable(test_serialization_erase test_serialization_erase.cpp)
target_link_libraries(test_serialization_erase protocol crdt)
add_test(NAME serialization_erase COMMAND test_serialization_erase "test owner" "test document" "C" 1 5 2 4 5)

add_executable(test_serialization_cursor test_serialization_cursor.cpp)
target_link_libraries(test_serialization_cursor protocol crdt)
add_test(NAME serialization_cursor COMMAND test_serialization_cursor "test owner" "test document" "test username" "C" 1 5 2 4 5)

add_executable(test_serialization_close test_serialization_close.cpp)
target_link_libraries(test_serialization_close protocol)
add_test(NAME serialization_close COMMAND test_serialization_close "test owner" "test document" "test username")

add_executable(test_serialization_profile test_serialization_profile.cpp)
target_link_libraries(test_serialization_profile protocol)
add_test(NAME serialization_profile COMMAND test_serialization_profile "test username" "test name" "test surname" "test password" 1 1 0 0 255)

add_executable(test_serialization_profile_ok test_serialization_profile_ok.cpp)
target_link_libraries(test_serialization_profile_ok protocol)
add_test(NAME serialization_profile_ok COMMAND test_serialization_profile_ok)