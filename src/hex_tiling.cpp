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
            hexagon.SetFamily(Randomizer::Random(2, 5));
    }

    void Tiling::Randomize(const std::vector<Index>& indices)
    {
        for(const auto& index : indices)
            tiles_.at(index).SetFamily(Randomizer::Random(2, 5));
    }

    bool Tiling::CheckEquality(const std::vector<Index>& indices)
    {
        bool allEqual = true;
        uint8_t value = tiles_.at(indices.front()).GetFamily();
        for(const auto& index : indices)
            allEqual &= (tiles_.at(index).GetFamily() == value);

        return allEqual;
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

    void Tiling::RotateWithMotion(Tiling::Index rotationCenter, int16_t rotationRadius, int16_t rotationSteps)
    {
        motion_.angularSpeed = 180;
        motion_.rotationCenter = rotationCenter;
        motion_.rotationSteps = rotationSteps;
        motion_.curentAngle = 0;
        motion_.stopAngle = rotationSteps * 60;
        motion_.rotationRadius = rotationRadius;
        motion_.lastTick = std::chrono::system_clock::now();
        animationActive_ = true;
    }

    std::tuple<bool, Tiling::Index> Tiling::GetHoveringIndex(Tiling::Pixel pixel)
    {
        Index index = PixelToIndex(pixel, hexRadius_, tileCenter_);
        bool isValid = tiles_.find(index) != tiles_.end();

        return {isValid, index};
    }

    double Tiling::RadiusToApothem(double radius)
    {
        return sqrt(3) / 2 * radius;
    }

    Tiling::Index Tiling::PixelToIndex(const Tiling::Pixel& pixel, double radius, Tiling::Pixel center)
    {
        double x = pixel.first - center.first;
        double y = pixel.second - center.second;

        double q = (sqrt(3.0) / 3.0 * x - 1.0 / 3.0 * y) / radius;
        double r = (2.0 / 3.0 * y) / radius;

        return RoundIndex(q, r, -q - r);
    }

    Tiling::Pixel Tiling::IndexToPixel(const Tiling::Index& index, double radius, Tiling::Pixel center)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        double x = center.first + (q * sqrt(3.0) + r * sqrt(3.0) / 2.0) * radius;
        double y = center.second + (3.0 / 2.0 * r) * radius;

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
        while(rotation < 0)
        {
            int16_t swap = q;
            q = -r;
            r = -s;
            s = -swap;
            rotation--;
        }
        while(rotation > 0)
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

    const std::map<Tiling::Index, Hexagon>& Tiling::GetTiles() const
    {
        return tiles_;
    }

    double WrapAngle(double angle)
    {
        while(angle >= 360)
            angle -= 360;
        while(angle < 0)
            angle += 360;
        return angle;
    }

    double AngularDistance(double A, double B)
    {
        A = WrapAngle(A) * M_PI / 180;
        B = WrapAngle(B) * M_PI / 180;

        return atan2(sin(A - B), cos(A - B)) * 180 / M_PI;
    }

    void Tiling::UpdatePhysics()
    {
        if(animationActive_)
        {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            int64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - motion_.lastTick).count();

            double angleDistance = AngularDistance(motion_.curentAngle, motion_.stopAngle);
            double angleStep = copysign(motion_.angularSpeed * static_cast<double>(elapsed) / 1e6, angleDistance);

            if(abs(angleDistance) < angleStep) angleStep = abs(angleDistance);
            motion_.curentAngle -= angleStep;

            if(abs(motion_.curentAngle - motion_.stopAngle) < 1e-6)
            {
                motion_.curentAngle = motion_.stopAngle;
                animationActive_ = false;
            }
            motion_.lastTick = now;

            double centerX = tiles_.at(motion_.rotationCenter).GetX();
            double centerY = tiles_.at(motion_.rotationCenter).GetY();
            for(auto& [index, hexagon] : tiles_)
                if(IndexDistance(motion_.rotationCenter, index) <= motion_.rotationRadius)
                    hexagon.Rotate(centerX, centerY, angleStep);

            if(!animationActive_)
            {
                std::map<Index, Hexagon> rotatedTiles;
                std::vector<Index> toBeRotated;

                for(const auto& [index, hexagon] : tiles_)
                    if(IndexDistance(motion_.rotationCenter, index) <= motion_.rotationRadius)
                        toBeRotated.push_back(index);

                for(const auto& index : toBeRotated)
                {
                    Index offset = index - motion_.rotationCenter;
                    Index rotated = RotateIndex(offset, motion_.rotationSteps);
                    Index corrected = rotated + motion_.rotationCenter;
                    ValidateIndex(corrected);

                    auto tile = tiles_.extract(index);
                    tile.key() = corrected;
                    Pixel updated = IndexToPixel(tile.key(), hexRadius_, tileCenter_);
                    tile.mapped().Update(updated.first, updated.second);
                    rotatedTiles.insert(std::move(tile));
                }
                tiles_.insert(rotatedTiles.begin(), rotatedTiles.end());
            }
        }
    }

} // namespace hex
