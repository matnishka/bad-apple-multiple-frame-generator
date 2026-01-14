cmake_minimum_required(VERSION 3.10)
project(BaB C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_executable(bag main.c)

if(MSVC)
    target_compile_options(bag PRIVATE /W4)
endif()