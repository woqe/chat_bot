cmake_minimum_required(VERSION 3.5)
project(petbot)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-m32")

set(CMAKE_BUILD_TYPE Debug)

include_directories(./)
include_directories(libs/botapi/header)
include_directories(libs/data)

include_directories(libs/openssl/include/)
include_directories(libs/openssl/include/openssl)

include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include/)

include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include)

link_directories(libs/openssl/lib)
link_directories(.)

link_directories(libs/botapi)
link_directories(libs/data)

subdirs(libs/botapi)
subdirs(libs/data)

set(SRC main.cpp func.h realize.cpp)
set(LIBS data botapi libssl libcrypto WS2_32)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIBS})
