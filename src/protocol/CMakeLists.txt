find_package(Qt5 COMPONENTS Core Gui REQUIRED)

list(APPEND SRC
        message.cpp
        profile.cpp
        document.cpp
        document_info.cpp
        error_message.cpp
        signup_message.cpp
        signup_ok_message.cpp
        login_message.cpp
        logout_message.cpp
        profile_message.cpp
        profile_ok_message.cpp
        documents_message.cpp
        create_message.cpp
        open_message.cpp
        close_message.cpp
        document_message.cpp
        insert_message.cpp
        erase_message.cpp
        cursor_message.cpp)

list(APPEND LIB Qt5::Core Qt5::Gui crdt)
list(APPEND INCLUDE)

add_library(protocol STATIC ${SRC})
target_link_libraries(protocol ${LIB})
target_include_directories(protocol PUBLIC ${INCLUDE})