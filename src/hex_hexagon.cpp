#include <algorithm>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{
    Hexagon::Hexagon(double x, double y, double radius, uint8_t family)
    {
        x_ = x;
        y_ = y;
        radius_ = radius;
        family_ = family;
        orientation_ = 0;
        UpdateVertices();
    }

    void Hexagon::Rotate(double angle)
    {
        orientation_ += angle;
        while(orientation_ >= 360)
            orientation_ -= 360;
        while(orientation_ < 0)
            orientation_ += 360;

        UpdateVertices();
    }

    void Hexagon::Rotate(double pointX, double pointY, double angle)
    {
        orientation_ += angle;
        while(orientation_ >= 360)
            orientation_ -= 360;
        while(orientation_ < 0)
            orientation_ += 360;

        double radAngle = angle * M_PI / 180;

        double x = cos(radAngle) * (x_ - pointX) - sin(radAngle) * (y_ - pointY) + pointX;
        double y = sin(radAngle) * (x_ - pointX) + cos(radAngle) * (y_ - pointY) + pointY;

        x_ = x;
        y_ = y;

        UpdateVertices();
    }

    void Hexagon::RotateTo(double pointX, double pointY, double orientation)
    {
        orientation_ = orientation;
        while(orientation_ >= 360)
            orientation_ -= 360;
        while(orientation_ < 0)
            orientation_ += 360;

        double radAngle = orientation_ * M_PI / 180.0;

        double x = cos(radAngle) * (x_ - pointX) + sin(radAngle) * (y_ - pointY) + pointX;
        double y = -sin(radAngle) * (x_ - pointX) + cos(radAngle) * (y_ - pointY) + pointY;

        x_ = x;
        y_ = y;

        UpdateVertices();
    }

    void Hexagon::Update(double x, double y)
    {
        x_ = x;
        y_ = y;
        UpdateVertices();
    }

    void Hexagon::UpdateVertices()
    {
        const int8_t X = 0;
        const int8_t Y = 1;

        for(int i = 0; i < 6; i++)
        {
            double radAngle = (orientation_ + 30.0 + i * 60.0) * M_PI / 180.0;

            vertices_[X][i] = std::cos(radAngle) * radius_ + x_;
            vertices_[Y][i] = std::sin(radAngle) * radius_ + y_;
        }
    }

    double Hexagon::Distance(double x, double y) const
    {
        return std::sqrt(std::pow(x_ - x, 2) + std::pow(y_ - y, 2));
    }

    std::array<std::array<int16_t, 6>, 2> Hexagon::GetVertices() const
    {
        return vertices_;
    }

    std::array<std::array<int16_t, 6>, 2> Hexagon::CalculateVertices(double orientation, double radius) const
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

    void Hexagon::Draw() const
    {
        Palette::Color c = GetHexagonColor();
        Palette::Color b = Palette::GetColor(Palette::Element::Background);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices_[0].data(), vertices_[1].data(), 6, c.r, c.g, c.b, c.a));
        SDL(polygonRGBA(Engine::gRenderer_, vertices_[0].data(), vertices_[1].data(), 6, b.r, b.g, b.b, b.a));
    }

    void Hexagon::Draw(Palette::Color color) const
    {
        Palette::Color b = Palette::GetColor(Palette::Element::Background);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices_[0].data(), vertices_[1].data(), 6, color.r, color.g,
                              color.b, color.a));
        SDL(polygonRGBA(Engine::gRenderer_, vertices_[0].data(), vertices_[1].data(), 6, b.r, b.g, b.b, b.a));
    }

    void Hexagon::DrawHighlight() const
    {
        std::array<std::array<int16_t, 6>, 2> vertices = CalculateVertices(orientation_, 1.2 * radius_);

        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);
        SDL(filledPolygonRGBA(Engine::gRenderer_, vertices[0].data(), vertices[1].data(), 6, f.r, f.g, f.b, f.a));
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

    uint8_t Hexagon::GetFamily()
    {
        return family_;
    }

    void Hexagon::SetFamily(uint8_t family)
    {
        family_ = family;
    }

    double Hexagon::GetX()
    {
        return x_;
    }

    double Hexagon::GetY()
    {
        return y_;
    }

} // namespace hex
