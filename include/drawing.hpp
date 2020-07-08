#pragma once

#include <SDL2/SDL.h>

#include "hexagon.hpp"

namespace hexvoid
{
    void DrawHorizontalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon);
    void DrawVerticalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon);

} // namespace hexvoid
