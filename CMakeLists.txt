cmake_minimum_required(VERSION 3.1.0)

project(Elegant86 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER /usr/bin/g++-10)

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

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Script REQUIRED)

add_executable(Elegant86 ${project_files})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(Elegant86 Qt5::Widgets Qt5::Script)
