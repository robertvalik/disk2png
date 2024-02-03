cmake_minimum_required(VERSION 3.27)
project(disk2png C)

set(CMAKE_C_STANDARD 11)

add_executable(disk2png main.c)
target_link_libraries(disk2png gd)
