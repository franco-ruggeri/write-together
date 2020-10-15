find_package(Qt5 COMPONENTS Core REQUIRED)
find_path(CRYPTOPP_H cryptlib.h HINTS /usr/local/include/cryptopp REQUIRED)
find_library(CRYPTOPP_LIB NAMES cryptlib.lib libcryptopp.a REQUIRED)

list(APPEND SRC password_utility.cpp)
list(APPEND LIB ${CRYPTOPP_LIB} Qt5::Core)
list(APPEND INCLUDE ${CRYPTOPP_H})

add_library(crypto STATIC ${SRC})
target_link_libraries(crypto ${LIB})
target_include_directories(crypto PUBLIC ${INCLUDE})