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
                Index index{q, r, s};
                ValidateIndex(index);
                Pixel pixel = IndexToPixel(index, hexRadius, tileCenter_);
                tiles_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius, family});
            }
        }
    }

    void Grid::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        Index selected = PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

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
        Index selected = PixelToIndex({cursorX, cursorY}, hexRadius_, tileCenter_);
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

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

    bool Grid::CheckSolution(Tiling::Index index)
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

    void Grid::ShuffleSolution(Tiling::Index index)
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

} // namespace hex
