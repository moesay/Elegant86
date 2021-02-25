#Testing script for Elegant86

cmake_minimum_required(VERSION 3.1.0)

project(Test VERSION 1.0.0 LANGUAGES CXX)

include_directories("../" "../include" )

file(GLOB project_files
    "*.hpp"
    "*.cpp"
    "../source/exc.cpp"
    "../source/labels.cpp"
    "../include/exc.h"
    "../include/labels.h"
    "../assembler/source/*.cpp"
    "../assembler/include/*.h"
    "../assembler/fpasm/*.cpp"
    "../assembler/fpasm/*.h"
    )

find_package(GTest REQUIRED)

add_executable(Test ${project_files})
gtest_discover_tests(Test)

target_link_libraries(Test Qt5::Script gtest gtest_main pthread)
