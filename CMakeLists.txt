cmake_minimum_required(VERSION 3.10)

project(VizualizatorMatematic)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FRAMEWORK
    "./Framework/*.cpp"
    "./Framework/*.h"
)
file(GLOB_RECURSE SRC_PARSER
    "./Parser/*.cpp"
    "./Parser/*.h"
)
file(GLOB_RECURSE SRC_MATH
    "./Math/*.cpp"
    "./Math/*.h"
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FRAMEWORK} ${SRC_PARSER} ${SRC_MATH})

source_group("Framework" FILES ${SRC_FRAMEWORK})
source_group("Parser" FILES ${SRC_PARSER})
source_group("Math" FILES ${SRC_MATH})
