cmake_minimum_required(VERSION 3.18)
project(EverybodyCodes2025)


file(GLOB DAY_SOURCES src/day*.cpp)
foreach(source ${DAY_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 23)
    set_property(TARGET ${name} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endforeach(source ${DAY_SOURCES})
