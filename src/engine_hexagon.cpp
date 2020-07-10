#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "engine.hpp"
#include "hexvoid.hpp"

namespace hexvoid
{

    // void Hexagon::DrawHorizontal(SDL_Renderer*& gRenderer, uint8_t r, uint8_t g, uint8_t b)
    // {
    //     int16_t vx[6], vy[6];

    //     const double sin30 = std::sin(30.0 * M_PI / 180.0);
    //     const double cos30 = std::cos(30.0 * M_PI / 180.0);

    //     int16_t xOffset = sin30 * radius;
    //     int16_t yOffset = cos30 * radius;

    //     vx[0] = x_ + radius;
    //     vy[0] = y;

    //     vx[1] = x_ + xOffset;
    //     vy[1] = y_ - yOffset;

    //     vx[2] = x_ - xOffset;
    //     vy[2] = y_ - yOffset;

    //     vx[3] = x_ - radius;
    //     vy[3] = y;

    //     vx[4] = x_ - xOffset;
    //     vy[4] = y_ + yOffset;

    //     vx[5] = x_ + xOffset;
    //     vy[5] = y_ + yOffset;

    //     filledPolygonRGBA(gRenderer, vx, vy, 6, r, g, b, 255);
    // }

    double Hexagon::Distance(int16_t x, int16_t y)
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    void Hexagon::Draw(SDL_Renderer*& gRenderer, const Palette& palette)
    {
        int16_t vx[6], vy[6];

        const double sin30 = std::sin(30.0 * M_PI / 180.0);
        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t xOffset = cos30 * radius_;
        int16_t yOffset = sin30 * radius_;

        vx[0] = x_;
        vy[0] = y_ + radius_;

        vx[1] = x_ - xOffset;
        vy[1] = y_ + yOffset;

        vx[2] = x_ - xOffset;
        vy[2] = y_ - yOffset;

        vx[3] = x_;
        vy[3] = y_ - radius_;

        vx[4] = x_ + xOffset;
        vy[4] = y_ - yOffset;

        vx[5] = x_ + xOffset;
        vy[5] = y_ + yOffset;

        Palette::Color color = palette.GetThemeColor(family_);
        filledPolygonRGBA(gRenderer, vx, vy, 6, color.r, color.g, color.b, 255);
    }

    void Hexagon::DrawBackground(SDL_Renderer*& gRenderer, const Palette& palette)
    {
        int16_t vx[6], vy[6];

        int16_t overdraw = radius_ + 5; // fix to be dynamic

        const double sin30 = std::sin(30.0 * M_PI / 180.0);
        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t xOffset = cos30 * overdraw;
        int16_t yOffset = sin30 * overdraw;

        vx[0] = x_;
        vy[0] = y_ + overdraw;

        vx[1] = x_ - xOffset;
        vy[1] = y_ + yOffset;

        vx[2] = x_ - xOffset;
        vy[2] = y_ - yOffset;

        vx[3] = x_;
        vy[3] = y_ - overdraw;

        vx[4] = x_ + xOffset;
        vy[4] = y_ - yOffset;

        vx[5] = x_ + xOffset;
        vy[5] = y_ + yOffset;

        Palette::Color color = palette.GetThemeColor(1);
        filledPolygonRGBA(gRenderer, vx, vy, 6, color.r, color.g, color.b, 255);
    }

} // namespace hexvoid
