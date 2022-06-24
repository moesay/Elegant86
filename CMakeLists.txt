cmake_minimum_required(VERSION 3.1.0)

project(Elegant86 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 10 OR GCC_VERSION VERSION_EQUAL 10))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 10 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
    if (NOT (CLANG_VERSION VERSION_GREATER 10 OR CLANG_VERSION VERSION_EQUAL 10))
        message(FATAL_ERROR "${PROJECT_NAME} requires clang 10 or greater.")
    endif ()
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++20.")
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories("include/" )

file(GLOB project_files
    "*.cpp"
    "*.h"
    "source/*.cpp"
    "include/*.h"
    "codeEditor/source/*.cpp"
    "codeEditor/include/*.h"
    "assembler/source/*.cpp"
    "assembler/include/*.h"
    "assembler/fpasm/*.cpp"
    "assembler/fpasm/*.h"
    "resources.qrc"
    )

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 COMPONENTS Widgets Script REQUIRED)

add_executable(Elegant86 ${project_files})

set_property(TARGET Elegant86 PROPERTY CXX_STANDARD 20)

target_link_libraries(Elegant86 Qt5::Widgets Qt5::Script)

#Testing setup
if(UNIX)
    enable_testing()
    add_subdirectory(tests)
endif()
