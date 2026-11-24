cmake_minimum_required(VERSION 3.20)
project(GameOfLife C)

set(CMAKE_C_STANDARD 11)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(raylib REQUIRED)

set(MAC_FRAMEWORKS "-framework IOKit" "-framework Cocoa" "-framework OpenGL")

add_executable(GameOfLife main.c)
target_link_libraries(GameOfLife PRIVATE raylib ${MAC_FRAMEWORKS})

add_executable(Graphics graphics.c)
target_link_libraries(Graphics PRIVATE raylib ${MAC_FRAMEWORKS})