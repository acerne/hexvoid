#include "drawing.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "color.hpp"

namespace hexvoid
{

    void DrawHorizontalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon)
    {
        Color color = GetColorFromTheme(hexvoid::LuminousLines, hexagon.color);

        int16_t vx[6], vy[6];

        const double sin30 = std::sin(30.0 * M_PI / 180.0);
        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t xOffset = sin30 * hexagon.radius;
        int16_t yOffset = cos30 * hexagon.radius;

        vx[0] = hexagon.x + hexagon.radius;
        vy[0] = hexagon.y;

        vx[1] = hexagon.x + xOffset;
        vy[1] = hexagon.y - yOffset;

        vx[2] = hexagon.x - xOffset;
        vy[2] = hexagon.y - yOffset;

        vx[3] = hexagon.x - hexagon.radius;
        vy[3] = hexagon.y;

        vx[4] = hexagon.x - xOffset;
        vy[4] = hexagon.y + yOffset;

        vx[5] = hexagon.x + xOffset;
        vy[5] = hexagon.y + yOffset;

        filledPolygonRGBA(gRenderer, vx, vy, 6, color.r, color.g, color.b, 255);
    }

    void DrawVerticalHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon)
    {
        Color color = GetColorFromTheme(hexvoid::LuminousLines, hexagon.color);

        int16_t vx[6], vy[6];

        const double sin30 = std::sin(30.0 * M_PI / 180.0);
        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t xOffset = cos30 * hexagon.radius;
        int16_t yOffset = sin30 * hexagon.radius;

        vx[0] = hexagon.x;
        vy[0] = hexagon.y + hexagon.radius;

        vx[1] = hexagon.x - xOffset;
        vy[1] = hexagon.y + yOffset;

        vx[2] = hexagon.x - xOffset;
        vy[2] = hexagon.y - yOffset;

        vx[3] = hexagon.x;
        vy[3] = hexagon.y - hexagon.radius;

        vx[4] = hexagon.x + xOffset;
        vy[4] = hexagon.y - yOffset;

        vx[5] = hexagon.x + xOffset;
        vy[5] = hexagon.y + yOffset;

        filledPolygonRGBA(gRenderer, vx, vy, 6, color.r, color.g, color.b, 255);
    }

} // namespace hexvoid
