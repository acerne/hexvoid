#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{
    void Tiling::Randomize()
    {
        for(auto& [index, hexagon] : tiles_)
            hexagon.family_ = Randomizer::Random(2, 5);
    }

    void Tiling::Rotate(Math::Index rotationCenter, int16_t rotationRadius, int16_t rotation)
    {
        std::map<Math::Index, Hexagon> rotatedTiles;
        std::vector<Math::Index> toBeRotated;

        for(const auto& [index, hexagon] : tiles_)
            if(Math::IndexDistance(rotationCenter, index) <= rotationRadius) toBeRotated.push_back(index);

        for(const auto& index : toBeRotated)
        {
            Math::Index offset = index - rotationCenter;
            Math::Index rotated = Math::RotateIndex(offset, rotation);
            Math::Index corrected = rotated + rotationCenter;
            Math::ValidateIndex(corrected);

            auto tile = tiles_.extract(index);
            tile.key() = corrected;
            Math::Pixel updated = Math::IndexToPixel(tile.key(), hexRadius_, tileCenter_);
            tile.mapped().Update(updated.first, updated.second);
            rotatedTiles.insert(std::move(tile));
        }
        tiles_.insert(rotatedTiles.begin(), rotatedTiles.end());
    }

} // namespace hex
