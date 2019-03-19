cmake_minimum_required(VERSION 3.3)

PROJECT (Square)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

SET(source_files Square.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIBRARY})

set(glad_dir ./glad)
add_subdirectory(${glad_dir})
include_directories(${glad_dir})

ADD_EXECUTABLE(Square ${source_files})
target_link_libraries(Square ${SDL2_LIBRARY} glad)