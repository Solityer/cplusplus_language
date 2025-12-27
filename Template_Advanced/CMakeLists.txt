cmake_minimum_required(VERSION 4.0)
project(iterator)

set(CMAKE_CXX_STANDARD 20)

add_executable(iterator main.cpp
        list.h
        reverse_iterator.h
        vector.h
        stack.cpp)
