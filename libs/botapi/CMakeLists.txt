project(botapi)

set(SRC source/bot.cpp header/bot.h)
set(CMAKE_CXX_FLAGS "-m32")

add_library(${PROJECT_NAME} STATIC ${SRC})

