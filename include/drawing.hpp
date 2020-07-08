#pragma once

#include <SDL2/SDL.h>

#include "hexagon.hpp"

namespace hexvoid
{
    void DrawHorizontalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon, uint8_t r, uint8_t g, uint8_t b);
    void DrawVerticalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon, uint8_t r, uint8_t g, uint8_t b);

} // namespace hexvoid
