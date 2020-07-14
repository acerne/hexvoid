#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{

    uint8_t Random(uint8_t min, uint8_t max)
    {
        static std::default_random_engine generator(time(0));
        std::uniform_int_distribution<uint8_t> distribution(min, max);
        return distribution(generator);
    }

    Grid::Grid(int16_t size, int16_t hexRadius)
    {
        if(debug_) printf("Creating Grid class...\n");

        if(!(size % 2)) throw std::runtime_error("Grid size must be an odd nubmer");

        gridSize_ = size;
        int16_t gridRadius = (size - 1) / 2;
        hexRadius_ = hexRadius;

        elements_.clear();

        screenCenter_.first = Engine::windowWidth_ / 2; // TODO center as argument
        screenCenter_.second = Engine::windowHeight_ / 2;

        for(int16_t q = -gridRadius + 1; q < gridRadius; q++)
        {
            for(int16_t r = std::max(-gridRadius + 1, -q - gridRadius + 1);
                r <= std::min(gridRadius - 1, -q + gridRadius - 1); r++)
            {
                int16_t s = -q - r;
                uint8_t family = Random(2, 5);
                Index index{q, r, s};
                Pixel pixel = IndexToPixel(index);
                elements_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius, family});
            }
        }
    }

    void Grid::Randomize()
    {
        for(auto& [index, hexagon] : elements_)
            hexagon.family_ = Random(2, 5);
    }

    void Grid::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        if(inside)
        {
            if(CheckSolution(selected))
            {
                printf("HIT!\n");
                ShuffleSolution(selected);

                // score_.AddScore(100);
                // score_.AddMoves(10);
            }
            else
            {
                int16_t q = std::get<0>(selected);
                int16_t r = std::get<1>(selected);
                int16_t s = std::get<2>(selected);

                uint8_t swap = elements_.at({q + 1, r - 1, s}).family_;
                elements_.at({q + 1, r - 1, s}).family_ = elements_.at({q, r - 1, s + 1}).family_;
                elements_.at({q, r - 1, s + 1}).family_ = elements_.at({q - 1, r, s + 1}).family_;
                elements_.at({q - 1, r, s + 1}).family_ = elements_.at({q - 1, r + 1, s}).family_;
                elements_.at({q - 1, r + 1, s}).family_ = elements_.at({q, r + 1, s - 1}).family_;
                elements_.at({q, r + 1, s - 1}).family_ = elements_.at({q + 1, r, s - 1}).family_;
                elements_.at({q + 1, r, s - 1}).family_ = swap;

                // score_.Move();
            }
        }
    }

    void Grid::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        if(inside)
        {
            if(CheckSolution(selected))
            {
                printf("HIT!\n");
                ShuffleSolution(selected);

                // score_.AddScore(100);
                // score_.AddMoves(10);
            }
            else
            {
                int16_t q = std::get<0>(selected);
                int16_t r = std::get<1>(selected);
                int16_t s = std::get<2>(selected);

                uint8_t swap = elements_.at({q + 1, r - 1, s}).family_;
                elements_.at({q + 1, r - 1, s}).family_ = elements_.at({q + 1, r, s - 1}).family_;
                elements_.at({q + 1, r, s - 1}).family_ = elements_.at({q, r + 1, s - 1}).family_;
                elements_.at({q, r + 1, s - 1}).family_ = elements_.at({q - 1, r + 1, s}).family_;
                elements_.at({q - 1, r + 1, s}).family_ = elements_.at({q - 1, r, s + 1}).family_;
                elements_.at({q - 1, r, s + 1}).family_ = elements_.at({q, r - 1, s + 1}).family_;
                elements_.at({q, r - 1, s + 1}).family_ = swap;

                // score_.Move();
            }
        }
    }

    void Grid::Draw(const Palette& palette, int16_t cursorX, int16_t cursorY) const
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < (gridSize_ - 1) / 2 - 1;

        int16_t q = std::get<0>(selected);
        int16_t r = std::get<1>(selected);
        int16_t s = std::get<2>(selected);

        std::vector<Index> topmost;
        topmost.reserve(6);

        for(const auto& [index, hexagon] : elements_)
        {
            if(inside && IndexDistance(index, selected) == 1)
            {
                topmost.push_back(index);
            }
            else
            {
                hexagon.Draw(palette);
            }
        }

        if(inside)
        {
            for(auto& index : topmost)
            {
                elements_.at(index).DrawHighlight(palette);
            }
            for(auto& index : topmost)
            {
                elements_.at(index).Draw(palette);
            }
        }
        // score_.Draw(gRenderer);
    }

    bool Grid::CheckSolution(Grid::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        uint8_t reference = elements_.at(index).family_;

        bool hit = true;
        hit &= reference == elements_.at({q + 1, r - 1, s}).family_;
        hit &= reference == elements_.at({q, r - 1, s + 1}).family_;
        hit &= reference == elements_.at({q - 1, r, s + 1}).family_;
        hit &= reference == elements_.at({q - 1, r + 1, s}).family_;
        hit &= reference == elements_.at({q, r + 1, s - 1}).family_;
        hit &= reference == elements_.at({q + 1, r, s - 1}).family_;

        return hit;
    }

    void Grid::ShuffleSolution(Grid::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        elements_.at(index).family_ = Random(2, 5);
        elements_.at({q + 1, r - 1, s}).family_ = Random(2, 5);
        elements_.at({q, r - 1, s + 1}).family_ = Random(2, 5);
        elements_.at({q - 1, r, s + 1}).family_ = Random(2, 5);
        elements_.at({q - 1, r + 1, s}).family_ = Random(2, 5);
        elements_.at({q, r + 1, s - 1}).family_ = Random(2, 5);
        elements_.at({q + 1, r, s - 1}).family_ = Random(2, 5);
    }

    Grid::Index Grid::PixelToIndex(const Grid::Pixel& pixel) const
    {
        int16_t x = pixel.first - screenCenter_.first;
        int16_t y = pixel.second - screenCenter_.second;

        double q = (sqrt(3.0) / 3.0 * x - 1.0 / 3.0 * y) / hexRadius_;
        double r = (2.0 / 3.0 * y) / hexRadius_;

        return Round(q, r, -q - r);
    }

    Grid::Pixel Grid::IndexToPixel(const Grid::Index& index) const
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        int16_t x = screenCenter_.first + (q * sqrt(3.0) + r * sqrt(3.0) / 2.0) * hexRadius_;
        int16_t y = screenCenter_.second + (3.0 / 2.0 * r) * hexRadius_;

        return {x, y};
    }

    Grid::Index Grid::Round(double q, double r, double s) const
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

    int16_t Grid::IndexDistance(const Index& A, const Index& B) const
    {
        int16_t qA = std::get<0>(A);
        int16_t rA = std::get<1>(A);
        int16_t sA = std::get<2>(A);

        int16_t qB = std::get<0>(B);
        int16_t rB = std::get<1>(B);
        int16_t sB = std::get<2>(B);

        return (abs(qA - qB) + abs(rA - rB) + abs(sA - sB)) / 2;
    }

    Grid::Index Grid::GetClosestSelection(const Grid::Pixel& pixel) const
    {
        Index hover = PixelToIndex(pixel);

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

} // namespace hex
