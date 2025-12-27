cmake_minimum_required(VERSION 4.0)
project(hash)

set(CMAKE_CXX_STANDARD 20)

add_executable(hash main.cpp
        HashTable.h
        MyUnorderedMap.h
        MyUnorderedSet.h
        HashTable_1.h)
