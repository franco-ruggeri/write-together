find_package(Qt5 COMPONENTS Core Gui REQUIRED)

list(APPEND SRC
        Message.cpp
        Profile.cpp
        Document.cpp
        DocumentData.cpp
        ErrorMessage.cpp
        SignupMessage.cpp
        SignupOkMessage.cpp
        LoginMessage.cpp
        LogoutMessage.cpp
        ProfileMessage.cpp
        ProfileOkMessage.cpp
        DocumentsMessage.cpp
        CreateMessage.cpp
        OpenMessage.cpp
        CloseMessage.cpp
        DocumentMessage.cpp
        InsertMessage.cpp
        EraseMessage.cpp
        CursorMessage.cpp)

list(APPEND LIB Qt5::Core Qt5::Gui crdt)
list(APPEND INCLUDE)

add_library(protocol STATIC ${SRC})
target_link_libraries(protocol ${LIB})
target_include_directories(protocol PUBLIC ${INCLUDE})