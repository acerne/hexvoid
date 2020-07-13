cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME hexvoid)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_GFX_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}      src/main.cpp 
                                    src/hex_engine.cpp
                                    src/hex_framerate.cpp
                                    src/hex_grid.cpp
                                    src/hex_hexagon.cpp
                                    src/hex_menu.cpp
                                    src/hex_palette.cpp
                                    src/hex_randomizer.cpp
                                    src/hex_systeminfo.cpp
                                    src/hex_text.cpp
                                    )

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${SDL2_TTF_LIBRARIES})

