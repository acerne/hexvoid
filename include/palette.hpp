#pragma once

#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace hexvoid
{
    class Palette
    {
    public:
        Palette();

        struct Color
        {
            Color(uint8_t r, uint8_t g, uint8_t b) {}
            Color(int32_t hexValue)
            {
                r = ((hexValue >> 16) & 0xFF);
                g = ((hexValue >> 8) & 0xFF);
                b = ((hexValue)&0xFF);
            }
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        struct Theme
        {
            std::string name;
            Color background;
            Color foreground;
            Color A;
            Color B;
            Color C;
            Color D;
        };

        Color GetThemeColor(int index) const;
        std::string GetThemeName() const;

        void NextTheme();
        void PreviousTheme();

        void DrawInfo(SDL_Renderer*& gRenderer);

    private:
        uint8_t selectedTheme_;
        std::vector<Theme> themeList_;
        TTF_Font* font_;
    };

} // namespace hexvoid
