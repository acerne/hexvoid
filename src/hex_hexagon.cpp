#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{

    void Hexagon::Update(int16_t x, int16_t y)
    {
        x_ = x;
        y_ = y;
    }

    double Hexagon::Distance(int16_t x, int16_t y) const
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    std::array<std::array<int16_t, 6>, 2> Hexagon::GetVertices(double orientation, double radius) const
    {
        const int8_t X = 0;
        const int8_t Y = 1;
        std::array<std::array<int16_t, 6>, 2> vertices;

        for(int i = 0; i < 6; i++)
        {
            double radAngle = (orientation + 30.0 + i * 60.0) * M_PI / 180.0;

            vertices[X][i] = std::cos(radAngle) * radius + x_;
            vertices[Y][i] = std::sin(radAngle) * radius + y_;
        }

        return std::move(vertices);
    }

    void Hexagon::Draw(double angle) const
    {
        std::array<std::array<int16_t, 6>, 2> vertices = GetVertices(angle, radius_);

        Palette::Color c = GetHexagonColor();
        Palette::Color b = Palette::GetColor(Palette::Element::Background);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, c.r, c.g, c.b, 255));
        SDL(polygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, b.r, b.g, b.b, 255));
    }

    void Hexagon::DrawHighlight(double angle) const
    {
        std::array<std::array<int16_t, 6>, 2> vertices = GetVertices(angle, 1.2 * radius_);

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
