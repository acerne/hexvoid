#include <algorithm>
#include <random>
#include <stdexcept>

#include "hexagon.hpp"

namespace hex
{

    Symbol::Symbol(Alphabet character, Math::Pixel center, double elementRadius)
    {
        tileCenter_ = center;
        hexRadius_ = elementRadius;
        switch(character)
        {
            case Alphabet::SEPARATOR:
                GenerateSeparator();
                break;
            case Alphabet::H:
                GenerateH();
                break;
            case Alphabet::E:
                GenerateE();
                break;
            case Alphabet::X:
                GenerateX();
                break;
            case Alphabet::V:
                GenerateV();
                break;
            case Alphabet::O:
                GenerateO();
                break;
            case Alphabet::I:
                GenerateI();
                break;
            case Alphabet::D:
                GenerateD();
                break;

            default:
                break;
        }
    }

    void Symbol::SetPosition(Math::Pixel center)
    {
        tileCenter_ = center;
        for(auto& element : tiles_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    void Symbol::Move(Math::Pixel movement)
    {
        tileCenter_.first += movement.first;
        tileCenter_.second += movement.second;
        for(auto& element : tiles_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    Math::Pixel Symbol::GetPosition() const
    {
        return tileCenter_;
    }

    int16_t Symbol::CalculateBestSpacing(const Symbol& right) const
    {
        // TODO: take in to account diagonal spacings as well
        int16_t spacing = 10;
        int16_t closest = 10;
        for(const auto& hexA : tiles_)
        {
            int16_t qA = std::get<0>(hexA.first);
            int16_t rA = std::get<1>(hexA.first);
            for(const auto& hexB : right.tiles_)
            {
                int16_t rB = std::get<1>(hexB.first);
                if(rA == rB)
                {
                    int16_t qB = std::get<0>(hexB.first);
                    int16_t apart = qB + spacing - qA;
                    if(apart < closest) closest = apart;
                }
            }
        }
        return spacing - closest + 2;
    }

    void Symbol::Draw() const
    {
        for(auto& element : tiles_)
            element.second.Draw();
    }

    void Symbol::AddHexagon(int16_t q, int16_t r)
    {
        Math::Index index{q, r, -q - r};
        Math::Pixel pixel = Math::IndexToPixel(index, hexRadius_, tileCenter_);
        tiles_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius_, 1});
    }

    void Symbol::GenerateH()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(3, -3);
        AddHexagon(4, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(2, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(0, 3);
        AddHexagon(1, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-1, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateE()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateX()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(3, -3);
        AddHexagon(4, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-1, -1);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(2, -1);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(0, 3);
        AddHexagon(1, 3);
        AddHexagon(-4, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateV()
    {
        AddHexagon(-1, -3);
        AddHexagon(4, -3);
        AddHexagon(-2, -2);
        AddHexagon(-1, -2);
        AddHexagon(3, -2);
        AddHexagon(4, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(1, 0);
        AddHexagon(2, 0);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-2, 2);
        AddHexagon(-1, 2);
        AddHexagon(0, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-2, 4);
    }

    void Symbol::GenerateO()
    {
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-3, 0);
        AddHexagon(-2, 0);
        AddHexagon(2, 0);
        AddHexagon(3, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
    }

    void Symbol::GenerateI()
    {
        AddHexagon(2, -3);
        AddHexagon(1, -3);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(0, 0);
        AddHexagon(-1, 0);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(-2, 2);
        AddHexagon(-1, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
    }

    void Symbol::GenerateD()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(2, 0);
        AddHexagon(3, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
    }

    void Symbol::GenerateSeparator()
    {

        AddHexagon(2, -4);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(1, -2);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(0, 0);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(-1, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-2, 4);
    }

} // namespace hex
