#pragma once

#include <cstddef>
#include <map>
#include <stdint.h>
#include <string>
#include <utility>
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
        // Hexagon(const Hexagon&);
        // Hexagon(Hexagon&&);

        double Distance(int16_t x, int16_t y) const;

        void Draw(SDL_Renderer*& gRenderer, const Palette& palette) const;
        void DrawBackground(SDL_Renderer*& gRenderer, const Palette& palette) const;

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
        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY) const;

    private:
        typedef std::pair<int16_t, int16_t> Index;

        std::map<Index, Hexagon> elements_;
        uint16_t clusterSize_;
        int16_t clusterRadius_;
        int16_t hexRadius_;
        int16_t clusterSpacing_;

        Index Match(int16_t x, int16_t y) const;
        int16_t UnitDistance(const Index& A, const Index& B) const;
    };

    class Framerate
    {
    public:
        Framerate();
        ~Framerate();

        void Tick();
        void Draw(SDL_Renderer*& gRenderer, const Palette& palette);

    private:
        uint16_t frameCount_;
        uint32_t lastSecond_;
        uint16_t fps_;
        TTF_Font* font_;
    };

} // namespace hexvoid
