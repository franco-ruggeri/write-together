find_package(Qt5 COMPONENTS Core Sql REQUIRED)

add_library(database DatabaseGuard.cpp db_utility.cpp)

target_link_libraries(database Qt5::Core Qt5::Sql)

target_include_directories(database PUBLIC include)