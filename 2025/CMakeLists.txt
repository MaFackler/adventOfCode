cmake_minimum_required(VERSION 3.18)
project(AdventOfCode2025)
# Libs
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.1
)
FetchContent_MakeAvailable(catch2)

# Project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB DAY_SOURCES src/day*.cpp)
foreach(source ${DAY_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 23)
    set_property(TARGET ${name} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endforeach(source ${DAY_SOURCES})

enable_testing()
include(Catch)
# catch_discover_tests(runtests)
