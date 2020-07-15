#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{
    double Hexagon::Distance(int16_t x, int16_t y) const
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    void Hexagon::Draw() const
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

        Palette::Color c = GetHexagonColor();
        Palette::Color b = Palette::GetColor(Palette::Element::Background);
        filledPolygonRGBA(Engine::gRenderer_, vx, vy, 6, c.r, c.g, c.b, 255);
        polygonRGBA(Engine::gRenderer_, vx, vy, 6, b.r, b.g, b.b, 255);
    }

    void Hexagon::DrawHighlight() const
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

        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);
        filledPolygonRGBA(Engine::gRenderer_, vx, vy, 6, f.r, f.g, f.b, 255);
    }

    Palette::Color Hexagon::GetHexagonColor() const
    {
        switch(family_)
        {
            case 2:
                return Palette::GetColor(Palette::Element::A);
            case 3:
                return Palette::GetColor(Palette::Element::B);
            case 4:
                return Palette::GetColor(Palette::Element::C);
            case 5:
                return Palette::GetColor(Palette::Element::D);
            default:
                break;
        }
        throw std::runtime_error("Hexagon color not specified");
    }

} // namespace hex
