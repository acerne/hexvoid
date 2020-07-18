#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{
    double Hexagon::Distance(int16_t x, int16_t y) const
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    std::array<std::array<int16_t, 6>, 2> Hexagon::GetVertices(Orientation orientation, double radius) const
    {
        const int8_t X = 0;
        const int8_t Y = 1;
        std::array<std::array<int16_t, 6>, 2> vertices;

        const double sin30 = std::sin(30.0 * M_PI / 180.0);
        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t shortOffset = cos30 * radius;
        int16_t longOffset = sin30 * radius;

        if(orientation == Orientation::Horizontal)
        {
            vertices[X][0] = x_;
            vertices[Y][0] = y_ + radius;

            vertices[X][1] = x_ - shortOffset;
            vertices[Y][1] = y_ + longOffset;

            vertices[X][2] = x_ - shortOffset;
            vertices[Y][2] = y_ - longOffset;

            vertices[X][3] = x_;
            vertices[Y][3] = y_ - radius;

            vertices[X][4] = x_ + shortOffset;
            vertices[Y][4] = y_ - longOffset;

            vertices[X][5] = x_ + shortOffset;
            vertices[Y][5] = y_ + longOffset;
        }
        else
        {
            vertices[X][0] = x_ + radius;
            vertices[Y][0] = y_;

            vertices[X][1] = x_ + longOffset;
            vertices[Y][1] = y_ - shortOffset;

            vertices[X][2] = x_ - longOffset;
            vertices[Y][2] = y_ - shortOffset;

            vertices[X][3] = x_ - radius;
            vertices[Y][3] = y_;

            vertices[X][4] = x_ - longOffset;
            vertices[Y][4] = y_ + shortOffset;

            vertices[X][5] = x_ + longOffset;
            vertices[Y][5] = y_ + shortOffset;
        }

        return std::move(vertices);
    }

    void Hexagon::Draw() const
    {
        std::array<std::array<int16_t, 6>, 2> vertices = GetVertices(Orientation::Horizontal, radius_);

        Palette::Color c = GetHexagonColor();
        Palette::Color b = Palette::GetColor(Palette::Element::Background);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, c.r, c.g, c.b, 255));
        SDL(polygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, b.r, b.g, b.b, 255));
    }

    void Hexagon::DrawHighlight() const
    {
        std::array<std::array<int16_t, 6>, 2> vertices = GetVertices(Orientation::Horizontal, 1.2 * radius_);

        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, f.r, f.g, f.b, 255));
    }

    Palette::Color Hexagon::GetHexagonColor() const
    {
        switch(family_)
        {
            case 0:
                return Palette::GetColor(Palette::Element::Background);
            case 1:
                return Palette::GetColor(Palette::Element::Foreground);
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
