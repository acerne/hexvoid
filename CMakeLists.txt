cmake_minimum_required(VERSION 3.10)

project(hexvoid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release)

find_package(SDL2 REQUIRED)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(hexvoid  src/main.cpp 
                        src/drawing.cpp
                        src/hexvoid.cpp
                        )

target_link_libraries(hexvoid ${SDL2_LIBRARIES})

