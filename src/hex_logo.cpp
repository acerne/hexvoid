#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{
    Logo::Logo()
    {
        elementRadius_ = 10;

        int16_t y = windowHeight_ / 2;

        logotype_.clear();
        logotype_.push_back(Symbol(Character::H, {70, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::E, {100, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::X, {100, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::V, {100, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::O, {100, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::I, {100, y}, elementRadius_));
        logotype_.push_back(Symbol(Character::D, {100, y}, elementRadius_));

        int16_t distanceFromFirstSymbol = 0;
        int16_t xFirstSymbol = logotype_.at(0).GetPosition().first;
        double hexWidth = 2 * Math::RadiusToApothem(elementRadius_);
        for(int i = 0; i < logotype_.size() - 1; i++)
        {
            distanceFromFirstSymbol += logotype_.at(i).CalculateBestSpacing(logotype_.at(i + 1));
            int16_t x = xFirstSymbol + distanceFromFirstSymbol * hexWidth;
            // printf("#%i:%i = %i -> (%i,%i) / %i\n", i, i + 1, centerDistance, x, y,
            //        x - logotype_.at(i).GetPosition().first);
            logotype_.at(i + 1).SetPosition({x, y});
        }
    }

    int16_t Logo::Symbol::CalculateBestSpacing(const Symbol& right) const
    {
        int16_t spacing = 10;
        int16_t closest = 10;
        for(const auto& hexA : elements_)
        {
            int16_t qA = std::get<0>(hexA.first);
            int16_t rA = std::get<1>(hexA.first);
            for(const auto& hexB : right.elements_)
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

    void Logo::Draw() const
    {
        for(auto& symbol : logotype_)
            symbol.Draw();
    }

    Logo::Symbol::Symbol(Character character, Math::Pixel center, double elementRadius)
    {
        center_ = center;
        elementRadius_ = elementRadius;
        switch(character)
        {
            case Character::H:
                GenerateH();
                break;
            case Character::E:
                GenerateE();
                break;
            case Character::X:
                GenerateX();
                break;
            case Character::V:
                GenerateV();
                break;
            case Character::O:
                GenerateO();
                break;
            case Character::I:
                GenerateI();
                break;
            case Character::D:
                GenerateD();
                break;

            default:
                break;
        }
    }

    void Logo::Symbol::SetPosition(Math::Pixel center)
    {
        center_ = center;
        for(auto& element : elements_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, elementRadius_, center_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    void Logo::Symbol::Move(Math::Pixel movement)
    {
        center_.first += movement.first;
        center_.second += movement.second;
        for(auto& element : elements_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, elementRadius_, center_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    Math::Pixel Logo::Symbol::GetPosition() const
    {
        return center_;
    }

    void Logo::Symbol::Draw() const
    {
        for(auto& element : elements_)
            element.second.Draw();
    }

    void Logo::Symbol::AddHexagon(int16_t q, int16_t r)
    {
        Math::Index index{q, r, -q - r};
        Math::Pixel pixel = Math::IndexToPixel(index, elementRadius_, center_);
        elements_.emplace(index, Hexagon{pixel.first, pixel.second, elementRadius_, 1});
    }

    void Logo::Symbol::GenerateH()
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

    void Logo::Symbol::GenerateE()
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

    void Logo::Symbol::GenerateX()
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

    void Logo::Symbol::GenerateV()
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

    void Logo::Symbol::GenerateO()
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

    void Logo::Symbol::GenerateI()
    {
        AddHexagon(2, -3);
        AddHexagon(1, -3);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(-1, 1);
        AddHexagon(-1, 1);
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

    void Logo::Symbol::GenerateD()
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

} // namespace hex
