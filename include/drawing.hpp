#pragma once

#include <SDL2/SDL.h>

#include "hexagon.hpp"

namespace hexvoid
{
    void DrawHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon);

} // namespace hexvoid
