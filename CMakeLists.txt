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

add_library(FontCache STATIC lib/SDL_FontCache/SDL_FontCache.c)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_GFX_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(lib/SDL_FontCache)

add_executable(${PROJECT_NAME}      src/main.cpp 
                                    src/hexvoid_game.cpp
                                    src/hex_engine.cpp
                                    src/hex_framerate.cpp
                                    src/hex_hexagon.cpp
                                    src/hex_input.cpp
                                    src/hex_logo.cpp
                                    src/hex_menu.cpp
                                    src/hex_palette.cpp
                                    src/hex_randomizer.cpp
                                    src/hex_score.cpp
                                    src/hex_systeminfo.cpp
                                    src/hex_tiling.cpp
                                    src/hex_tiling_grid.cpp
                                    src/hex_tiling_symbol.cpp
                                    )

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} FontCache ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${SDL2_TTF_LIBRARIES} )

