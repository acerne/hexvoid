#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{

    HexagonalGrid::HexagonalGrid(int16_t size, double hexRadius)
    {
        if(!(size % 2)) throw std::runtime_error("Grid size must be an odd nubmer");

        gridSize_ = size;
        int16_t gridRadius = (size - 1) / 2;
        hexRadius_ = hexRadius;

        tiles_.clear();

        tileCenter_.first = Engine::windowWidth_ / 2; // TODO center as argument
        tileCenter_.second = Engine::windowHeight_ / 2;

        for(int16_t q = -gridRadius + 1; q < gridRadius; q++)
        {
            for(int16_t r = std::max(-gridRadius + 1, -q - gridRadius + 1);
                r <= std::min(gridRadius - 1, -q + gridRadius - 1); r++)
            {
                int16_t s = -q - r;
                uint8_t family = Randomizer::Random(2, 5);
                Index index{q, r, s};
                ValidateIndex(index);
                auto [x, y] = IndexToPixel(index, hexRadius, tileCenter_);
                tiles_.emplace(index, Hexagon{x, y, hexRadius, family});
            }
        }
    }

    int16_t HexagonalGrid::GetGridSize()
    {
        return gridSize_;
    }

    void HexagonalGrid::Draw(int16_t cursorX, int16_t cursorY) const
    {

        Index selected = PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        int16_t q = std::get<0>(selected);
        int16_t r = std::get<1>(selected);
        int16_t s = std::get<2>(selected);

        std::vector<Index> topmost;
        topmost.reserve(6);

        for(const auto& [index, hexagon] : tiles_)
        {
            if(inside && IndexDistance(index, selected) == 1)
                topmost.push_back(index);
            else
                hexagon.Draw();
        }

        if(inside)
        {
            for(auto& index : topmost)
            {
                tiles_.at(index).DrawHighlight();
            }
            for(auto& index : topmost)
            {
                tiles_.at(index).Draw();
            }
        }
    }

} // namespace hex
