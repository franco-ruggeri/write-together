find_package(Qt5 COMPONENTS Core Sql REQUIRED)

list(APPEND SRC DatabaseGuard.cpp db_utility.cpp)
list(APPEND LIB Qt5::Core Qt5::Sql)
list(APPEND INCLUDE include)

add_library(database STATIC ${SRC})
target_link_libraries(database ${LIB})
target_include_directories(database PUBLIC ${INCLUDE})