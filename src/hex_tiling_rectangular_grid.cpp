#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{

    RectangularGrid::RectangularGrid(int16_t sizeX, int16_t sizeY, double hexRadius)
    {
        if(!(sizeX % 2) || !(sizeY % 2)) throw std::runtime_error("Grid size must be an odd nubmer");

        gridSizeX_ = sizeX;
        gridSizeY_ = sizeY;
        hexRadius_ = hexRadius;

        int16_t halfWidth = (gridSizeX_ - 1) / 2;
        int16_t halfHeight = (gridSizeY_ - 1) / 2;

        tiles_.clear();

        tileCenter_.first = Engine::windowWidth_ / 2; // TODO center as argument
        tileCenter_.second = Engine::windowHeight_ / 2;
        for(int16_t r = -halfHeight; r <= halfHeight; r++)
        {
            int16_t offset = r / 2 + (r < 0) * (r % 2);

            for(int16_t q = -halfWidth - offset; q <= halfWidth - offset; q++)
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

    void RectangularGrid::Draw() const
    {
        for(const auto& [index, hexagon] : tiles_)
            hexagon.Draw();
    }

} // namespace hex
