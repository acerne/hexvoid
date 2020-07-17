#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{
    Logo::Logo()
    {
        logotype_.clear();
        logotype_.push_back(Symbol(Character::O));
        logotype_.push_back(Symbol(Character::I));
    }

    Logo::Symbol::Symbol(Logo::Character character)
    {
        switch(character)
        {
            case Character::I:
                GenerateI();
                break;
            case Character::O:
                GenerateO();
                break;

            default:
                break;
        }
    }

    void Logo::Symbol::Draw()
    {
        for(auto& element : elements_)
            element.second.Draw();
    }

    void Logo::Symbol::AddHexagon(Math::Index index)
    {
        int16_t radius = 5;
        Math::Pixel pixel = Math::IndexToPixel(index, radius, center_);
        elements_.emplace(index, Hexagon{pixel.first, pixel.second, radius, Randomizer::Random(2, 5)});
    }

    void Logo::Symbol::GenerateI()
    {
        center_ = {450, 300};
        AddHexagon({2, -3, 1});
        AddHexagon({1, -3, 2});
        AddHexagon({0, -2, 2});
        AddHexagon({1, -2, 1});
        AddHexagon({0, -1, 1});
        AddHexagon({1, -1, 0});
        AddHexagon({-1, 1, 0});
        AddHexagon({-1, 1, 0});
        AddHexagon({0, 0, 0});
        AddHexagon({-1, 0, 1});
        AddHexagon({-1, 1, 0});
        AddHexagon({0, 1, -1});
        AddHexagon({-2, 2, 1});
        AddHexagon({-1, 2, -1});
        AddHexagon({-2, 3, -1});
        AddHexagon({-1, 3, -2});
        AddHexagon({-3, 4, -1});
        AddHexagon({-2, 4, -2});
    }

    void Logo::Symbol::GenerateO()
    {
        center_ = {400, 300};
        AddHexagon({0, -3, 3});
        AddHexagon({1, -3, 2});
        AddHexagon({2, -3, 1});
        AddHexagon({3, -3, 0});
        AddHexagon({-1, -2, 3});
        AddHexagon({0, -2, 2});
        AddHexagon({1, -2, 1});
        AddHexagon({2, -2, 0});
        AddHexagon({3, -2, 1});
        AddHexagon({-2, -1, 3});
        AddHexagon({-1, -1, 2});
        AddHexagon({2, -1, -1});
        AddHexagon({3, -1, -2});
        AddHexagon({-3, 0, 3});
        AddHexagon({-2, 0, 2});
        AddHexagon({2, 0, -2});
        AddHexagon({3, 0, -3});
        AddHexagon({-3, 1, 2});
        AddHexagon({-2, 1, 1});
        AddHexagon({1, 1, -2});
        AddHexagon({2, 1, -3});
        AddHexagon({-3, 2, 1});
        AddHexagon({-2, 2, 0});
        AddHexagon({0, 2, -2});
        AddHexagon({1, 2, -3});
        AddHexagon({-3, 3, 0});
        AddHexagon({-2, 3, -1});
        AddHexagon({-1, 3, -2});
        AddHexagon({0, 3, -3});
        AddHexagon({-3, 4, -1});
        AddHexagon({-2, 4, -2});
        AddHexagon({-1, 4, -3});
    }

    void Logo::Draw()
    {
        for(auto& symbol : logotype_)
            symbol.Draw();
    }

} // namespace hex
