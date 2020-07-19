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

    void Tiling::Rotate(Tiling::Index rotationCenter, int16_t rotationRadius, int16_t rotation)
    {
        std::map<Index, Hexagon> rotatedTiles;
        std::vector<Index> toBeRotated;

        for(const auto& [index, hexagon] : tiles_)
            if(IndexDistance(rotationCenter, index) <= rotationRadius) toBeRotated.push_back(index);

        for(const auto& index : toBeRotated)
        {
            Index offset = index - rotationCenter;
            Index rotated = RotateIndex(offset, rotation);
            Index corrected = rotated + rotationCenter;
            ValidateIndex(corrected);

            auto tile = tiles_.extract(index);
            tile.key() = corrected;
            Pixel updated = IndexToPixel(tile.key(), hexRadius_, tileCenter_);
            tile.mapped().Update(updated.first, updated.second);
            rotatedTiles.insert(std::move(tile));
        }
        tiles_.insert(rotatedTiles.begin(), rotatedTiles.end());
    }

    double Tiling::RadiusToApothem(double radius)
    {
        return sqrt(3) / 2 * radius;
    }

    Tiling::Index Tiling::PixelToIndex(const Tiling::Pixel& pixel, double radius, Tiling::Pixel center)
    {
        int16_t x = pixel.first - center.first;
        int16_t y = pixel.second - center.second;

        double q = (sqrt(3.0) / 3.0 * x - 1.0 / 3.0 * y) / radius;
        double r = (2.0 / 3.0 * y) / radius;

        return RoundIndex(q, r, -q - r);
    }

    Tiling::Pixel Tiling::IndexToPixel(const Tiling::Index& index, double radius, Tiling::Pixel center)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        int16_t x = center.first + (q * sqrt(3.0) + r * sqrt(3.0) / 2.0) * radius;
        int16_t y = center.second + (3.0 / 2.0 * r) * radius;

        return {x, y};
    }

    Tiling::Index Tiling::RoundIndex(double q, double r, double s)
    {
        int16_t rq = round(q);
        int16_t rr = round(r);
        int16_t rs = round(s);

        double dq = abs(rq - q);
        double dr = abs(rr - r);
        double ds = abs(rs - s);

        if(dq > dr && dq > ds)
            rq = -rr - rs;
        else if(dr > ds)
            rr = -rq - rs;
        else
            rs = -rq - rr;

        return {rq, rr, rs};
    }

    int16_t Tiling::IndexDistance(const Index& A, const Index& B)
    {
        int16_t qA = std::get<0>(A);
        int16_t rA = std::get<1>(A);
        int16_t sA = std::get<2>(A);

        int16_t qB = std::get<0>(B);
        int16_t rB = std::get<1>(B);
        int16_t sB = std::get<2>(B);

        return (abs(qA - qB) + abs(rA - rB) + abs(sA - sB)) / 2;
    }

    Tiling::Index Grid::GetClosestSelection(const Tiling::Pixel& pixel) const
    {
        Index hover = PixelToIndex(pixel, hexRadius_, tileCenter_);

        while(IndexDistance({0, 0, 0}, hover) >= (gridSize_ - 1) / 2 - 1)
        {
            int16_t q = std::get<0>(hover);
            int16_t r = std::get<1>(hover);
            int16_t s = std::get<2>(hover);

            if(abs(q) > abs(r) && abs(q) > abs(s))
            {
                (q < 0) ? q++ : q--;
                if(abs(r) > abs(s))
                    (r < 0) ? r++ : r--;
                else
                    (s < 0) ? s++ : s--;
            }
            else if(abs(r) > abs(s))
            {
                (r < 0) ? r++ : r--;
                if(abs(q) > abs(s))
                    (q < 0) ? q++ : q--;
                else
                    (s < 0) ? s++ : s--;
            }
            else
            {
                (s < 0) ? s++ : s--;
                if(abs(q) > abs(r))
                    (q < 0) ? q++ : q--;
                else
                    (r < 0) ? r++ : r--;
            }
            std::get<0>(hover) = q;
            std::get<1>(hover) = r;
            std::get<2>(hover) = s;
        }
        return hover;
    }

    Tiling::Index Tiling::RotateIndex(const Index& index, int16_t rotation)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);
        while(rotation > 0)
        {
            int16_t swap = q;
            q = -r;
            r = -s;
            s = -swap;
            rotation--;
        }
        while(rotation < 0)
        {
            int16_t swap = q;
            q = -s;
            s = -r;
            r = -swap;
            rotation++;
        }
        return {q, r, s};
    }

    void Tiling::ValidateIndex(const Index& index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);
        if(q + r + s != 0)
            throw std::runtime_error("Invalid index state : q=" + std::to_string(q) + ", r=" + std::to_string(r)
                                     + ", s=" + std::to_string(s));
    }

} // namespace hex
