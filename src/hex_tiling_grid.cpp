#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hexagon.hpp"

namespace hex
{

    Grid::Grid(int16_t size, double hexRadius)
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
                Math::Index index{q, r, s};
                Math::ValidateIndex(index);
                Math::Pixel pixel = Math::IndexToPixel(index, hexRadius, tileCenter_);
                tiles_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius, family});
            }
        }
    }

    void Grid::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        Math::Index selected = Math::PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = Math::IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        if(inside)
        {
            if(CheckSolution(selected))
            {
                Score::AddScore(100);
                Score::AddMoves(10);
                ShuffleSolution(selected);
            }
            else
            {
                Score::TakeMoves(1);
                Rotate(selected, 1, 1);
            }
            Score::RegisterMove();
        }
    }

    void Grid::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        Math::Index selected = Math::PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = Math::IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        if(inside)
        {
            if(CheckSolution(selected))
            {
                Score::AddScore(100);
                Score::AddMoves(10);
                ShuffleSolution(selected);
            }
            else
            {
                Score::TakeMoves(1);
                Rotate(selected, 1, -1);
            }
        }
        Score::RegisterMove();
    }

    void Grid::Draw(int16_t cursorX, int16_t cursorY) const
    {
        Math::Index selected = Math::PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = Math::IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        int16_t q = std::get<0>(selected);
        int16_t r = std::get<1>(selected);
        int16_t s = std::get<2>(selected);

        std::vector<Math::Index> topmost;
        topmost.reserve(6);

        for(const auto& [index, hexagon] : tiles_)
        {
            if(inside && Math::IndexDistance(index, selected) == 1)
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

    bool Grid::CheckSolution(Math::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        uint8_t reference = tiles_.at(index).family_;

        bool hit = true;
        hit &= reference == tiles_.at({q + 1, r - 1, s}).family_;
        hit &= reference == tiles_.at({q, r - 1, s + 1}).family_;
        hit &= reference == tiles_.at({q - 1, r, s + 1}).family_;
        hit &= reference == tiles_.at({q - 1, r + 1, s}).family_;
        hit &= reference == tiles_.at({q, r + 1, s - 1}).family_;
        hit &= reference == tiles_.at({q + 1, r, s - 1}).family_;

        return hit;
    }

    void Grid::ShuffleSolution(Math::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        tiles_.at(index).family_ = Randomizer::Random(2, 5);
        tiles_.at({q + 1, r - 1, s}).family_ = Randomizer::Random(2, 5);
        tiles_.at({q, r - 1, s + 1}).family_ = Randomizer::Random(2, 5);
        tiles_.at({q - 1, r, s + 1}).family_ = Randomizer::Random(2, 5);
        tiles_.at({q - 1, r + 1, s}).family_ = Randomizer::Random(2, 5);
        tiles_.at({q, r + 1, s - 1}).family_ = Randomizer::Random(2, 5);
        tiles_.at({q + 1, r, s - 1}).family_ = Randomizer::Random(2, 5);
    }

    double Math::RadiusToApothem(double radius)
    {
        return sqrt(3) / 2 * radius;
    }

    Math::Index Math::PixelToIndex(const Math::Pixel& pixel, double radius, Math::Pixel center)
    {
        int16_t x = pixel.first - center.first;
        int16_t y = pixel.second - center.second;

        double q = (sqrt(3.0) / 3.0 * x - 1.0 / 3.0 * y) / radius;
        double r = (2.0 / 3.0 * y) / radius;

        return RoundIndex(q, r, -q - r);
    }

    Math::Pixel Math::IndexToPixel(const Math::Index& index, double radius, Math::Pixel center)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        int16_t x = center.first + (q * sqrt(3.0) + r * sqrt(3.0) / 2.0) * radius;
        int16_t y = center.second + (3.0 / 2.0 * r) * radius;

        return {x, y};
    }

    Math::Index Math::RoundIndex(double q, double r, double s)
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

    int16_t Math::IndexDistance(const Index& A, const Index& B)
    {
        int16_t qA = std::get<0>(A);
        int16_t rA = std::get<1>(A);
        int16_t sA = std::get<2>(A);

        int16_t qB = std::get<0>(B);
        int16_t rB = std::get<1>(B);
        int16_t sB = std::get<2>(B);

        return (abs(qA - qB) + abs(rA - rB) + abs(sA - sB)) / 2;
    }

    Math::Index Grid::GetClosestSelection(const Math::Pixel& pixel) const
    {
        Math::Index hover = Math::PixelToIndex(pixel, hexRadius_, tileCenter_);

        while(Math::IndexDistance({0, 0, 0}, hover) >= (gridSize_ - 1) / 2 - 1)
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

    Math::Index Math::RotateIndex(const Index& index, int16_t rotation)
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

    void Math::ValidateIndex(const Index& index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);
        if(q + r + s != 0)
            throw std::runtime_error("Invalid index state : q=" + std::to_string(q) + ", r=" + std::to_string(r)
                                     + ", s=" + std::to_string(s));
    }

} // namespace hex
