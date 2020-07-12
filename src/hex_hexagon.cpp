#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{
    Hexagon::Hexagon(int16_t x, int16_t y, int16_t radius, uint8_t family) :
        x_(x), y_(y), radius_(radius), family_(family)
    {
        if(debug_) printf("Creating Hexagon class...\n");
    }

    double Hexagon::Distance(int16_t x, int16_t y) const
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    void Hexagon::Draw(const Palette& palette) const
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
        Palette::Color background = palette.GetThemeColor(0);
        filledPolygonRGBA(Engine::gRenderer_, vx, vy, 6, color.r, color.g, color.b, 255);
        polygonRGBA(Engine::gRenderer_, vx, vy, 6, background.r, background.g, background.b, 255);
    }

    void Hexagon::DrawHighlight(const Palette& palette) const
    {
        int16_t vx[6], vy[6];

        int16_t overdraw = radius_ * 1.2;

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
        filledPolygonRGBA(Engine::gRenderer_, vx, vy, 6, color.r, color.g, color.b, 255);
    }

} // namespace hex
