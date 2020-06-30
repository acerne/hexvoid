#pragma once

#include <stdexcept>
#include <stdint.h>

namespace hexvoid
{

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
        Color background;
        Color A;
        Color B;
        Color C;
        Color D;
    };

    // source: https://www.shutterstock.com/blog/neon-color-palettes
    Theme GeometricGlow{Color(0x000000), Color(0x08F7FE), Color(0x09FBBB), Color(0xFE53BB), Color(0xF5D300)};
    Theme NeonLights{Color(0x000000), Color(0xFFACFC), Color(0xF148FB), Color(0x7122FA), Color(0x560A86)};
    Theme PsychedelicPattern{Color(0x000000), Color(0x75D5FD), Color(0xB76CFD), Color(0xFF2281), Color(0x011FFD)};
    Theme LuminousLines{Color(0x000000), Color(0x01FFC3), Color(0x01FFFF), Color(0xFFB3FD), Color(0x9D72FF)};

    Color GetColorFromTheme(const Theme& theme, int index)
    {
        switch(index)
        {
            case 0:
                return theme.background;
            case 1:
                return theme.A;
            case 2:
                return theme.B;
            case 3:
                return theme.C;
            case 4:
                return theme.D;
            default:
                throw std::runtime_error("Invalid color index");
        }
    }

} // namespace hexvoid
