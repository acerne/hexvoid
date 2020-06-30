#pragma once

#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>

#include "drawing.hpp"

namespace hexvoid
{
    // Screen dimension constants
    const size_t WINDOW_WIDTH = 800;
    const size_t WINDOW_HEIGHT = 600;

    // Starts up SDL and creates window
    void Initialize(SDL_Window*& gWindow, SDL_Surface*& gSurface);

    // Loads media
    void LoadMedia(const std::string& fileName, SDL_Surface*& gBackground);

} // namespace hexvoid
