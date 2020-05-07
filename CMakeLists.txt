cmake_minimum_required(VERSION 3.16)
project(lazyfoo)

set(CMAKE_CXX_STANDARD 14)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)

add_executable(lazyfoo ${SOURCE_FILES})
target_link_libraries(lazyfoo ${SDL2_LIBRARY})
#add_executable(lazyfoo main.cpp)