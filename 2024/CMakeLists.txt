cmake_minimum_required(VERSION 3.18)
project(AdventOfCode2024)

# Raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)

FetchContent_MakeAvailable(raylib)

# set_property(GLOBAL PROPERTY CXX_STANDARD 23)
# set_property(GLOBAL PROPERTY COMPILE_WARNING_AS_ERROR ON)

file(GLOB DAY_SOURCES src/day*.cpp)
foreach(source ${DAY_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 23)
    set_property(TARGET ${name} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endforeach(source ${DAY_SOURCES})

file(GLOB DAY_SOURCES src/gday*.cpp)
foreach(source ${DAY_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 23)
    set_property(TARGET ${name} PROPERTY COMPILE_WARNING_AS_ERROR ON)
    target_include_directories(${name} PRIVATE ${raylib_SOURCE_DIR}/src)
    target_link_libraries(${name} PRIVATE raylib)
endforeach(source ${DAY_SOURCES})

add_executable(demo src/demo.cpp)
set_property(TARGET demo PROPERTY CXX_STANDARD 23)
set_property(TARGET demo PROPERTY COMPILE_WARNING_AS_ERROR ON)
