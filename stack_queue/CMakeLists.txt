cmake_minimum_required(VERSION 4.0)
project(stack_queue)

set(CMAKE_CXX_STANDARD 20)

add_executable(stack_queue main.cpp
        stack.h
        queue.h
        priority_queue.h)
