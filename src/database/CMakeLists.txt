find_package(Qt5 COMPONENTS Core Sql REQUIRED)

# Note: do not add db_utility_secret.cpp, see comment in db_utility_secret.h
list(APPEND SRC database_guard.cpp database_utility.cpp)
list(APPEND LIB Qt5::Core Qt5::Sql)
list(APPEND INCLUDE include)

add_library(database STATIC ${SRC})
target_link_libraries(database ${LIB})
target_include_directories(database PUBLIC ${INCLUDE})