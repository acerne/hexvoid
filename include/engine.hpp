#pragma once

#include <cstddef>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "hexagon.hpp"

namespace hexvoid
{
    class FPS
    {
    public:
        FPS();
        ~FPS();

        void Tick();
        void Draw(SDL_Renderer*& gRenderer);

    private:
        uint16_t frameCount_;
        uint32_t lastSecond_;
        uint16_t fps_;
        TTF_Font* font_;
        SDL_Color textColor_;
    };

    uint8_t Random(uint8_t min, uint8_t max);

    std::vector<Hexagon> GenerateHexagonGrid(int16_t gridColumns, int16_t gridRows, int16_t hexRadius, int16_t spacing);
    Hexagon FindClosestHexagon(const std::vector<Hexagon>& grid, int16_t x, int16_t y);

} // namespace hexvoid
