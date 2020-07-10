#pragma once

#include <cstddef>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "palette.hpp"

namespace hexvoid
{
    class Hexagon
    {
    public:
        Hexagon(int16_t x, int16_t y, int16_t radius, uint8_t family) : x_(x), y_(y), radius_(radius), family_(family)
        {}

        double Distance(int16_t x, int16_t y);

        // void DrawHorizontal(SDL_Renderer*& gRenderer, uint8_t r, uint8_t g,
        // uint8_t b);
        void Draw(SDL_Renderer*& gRenderer, const Palette& palette);
        void DrawBackground(SDL_Renderer*& gRenderer, const Palette& palette);

        int16_t x_;
        int16_t y_;
        int16_t radius_;
        uint8_t family_;
    };

    class Cluster
    {
    public:
        Cluster(int16_t radius, int16_t hexRadius, int16_t spacing);

        void Randomize();
        void Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY);

    private:
        std::vector<Hexagon> elements_;
        uint16_t clusterSize_;
        int16_t clusterRadius_;
        int16_t hexRadius_;
        int16_t clusterSpacing_;

        uint16_t FindClosestElement(int16_t x, int16_t y);
    };

    class Framerate
    {
    public:
        Framerate();
        ~Framerate();

        void Tick();
        void Draw(SDL_Renderer*& gRenderer);

    private:
        uint16_t frameCount_;
        uint32_t lastSecond_;
        uint16_t fps_;
        TTF_Font* font_;
        SDL_Color textColor_;
    };

} // namespace hexvoid
