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
    Theme VividFerns{Color(0x000000), Color(0xFDC7D7), Color(0xFF9DE6), Color(0xA5D8F3), Color(0xE8E500)};
    Theme AbstractWall{Color(0x000000), Color(0x00FECA), Color(0xFDF200), Color(0xFF85EA), Color(0x7B61F8)};
    Theme ColorfulCottage{Color(0x000000), Color(0xFFD300), Color(0xDE38C8), Color(0x652EC7), Color(0x33135C)};
    Theme CosmicDust{Color(0x000000), Color(0x3B27BA), Color(0xE847AE), Color(0x13CA91), Color(0xFF9472)};
    Theme PopsOfPink{Color(0x000000), Color(0xFFDEF3), Color(0xFF61BE), Color(0x3B55CE), Color(0x35212A)};
    Theme FlourescentFish{Color(0x000000), Color(0xFEC763), Color(0xEA55B1), Color(0xA992FA), Color(0x00207F)};

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
