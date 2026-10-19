cmake_minimum_required(VERSION 4.1)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project1 src/main.cpp
        src/GatorAVL.cpp
        src/GatorAVL.h
        src/parsing.h
        src/parsing.cpp)
