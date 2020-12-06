project(data)

set(SRC Item.cpp Item.h)
set(CMAKE_CXX_FLAGS "-m32")

add_library(${PROJECT_NAME} STATIC ${SRC})

