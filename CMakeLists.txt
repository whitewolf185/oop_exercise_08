cmake_minimum_required(VERSION 3.17)
project(oop_exercise_08)

set(CMAKE_CXX_STANDARD 17)

add_executable(oop_exercise_08 main.cpp event_loop.h event.h)
add_executable(test test.cpp)