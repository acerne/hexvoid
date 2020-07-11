#pragma once

#include <cstddef>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace hexvoid
{
    class Text
    {
    public:
        Text(int fontSize);
        ~Text();

        void Position(int16_t x, int16_t y);
        void Apply(const std::string& text);

        void Draw(SDL_Renderer*& gRenderer);

    private:
        TTF_Font* font_;
        std::string text_;
        int16_t x_;
        int16_t y_;
    };

} // namespace hexvoid