cmake_minimum_required(VERSION 3.3)

PROJECT (Maze)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIBRARY})

set(glad_dir ./glad)
add_subdirectory(${glad_dir})
include_directories(${glad_dir})

ADD_EXECUTABLE(Maze ${source_files})
target_link_libraries(Maze ${SDL2_LIBRARY} glad)
target_link_libraries(Maze ${SDL2_LIBRARIES})