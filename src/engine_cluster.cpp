#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "engine.hpp"
#include "hexvoid.hpp"
#include "palette.hpp"

namespace hexvoid
{

    uint8_t Random(uint8_t min, uint8_t max)
    {
        static std::default_random_engine generator(time(0));
        std::uniform_int_distribution<uint8_t> distribution(min, max);
        return distribution(generator);
    }

    Cluster::Cluster(int16_t radius, int16_t hexRadius, size_t canvasWidth, size_t canvasHeight)
    {
        clusterRadius_ = radius;
        hexRadius_ = hexRadius;

        elements_.clear();

        screenCenter_.first = canvasWidth / 2;
        screenCenter_.second = canvasHeight / 2;

        for(int16_t q = -radius + 1; q < radius; q++)
        {
            for(int16_t r = std::max(-radius + 1, -q - radius + 1); r <= std::min(radius - 1, -q + radius - 1); r++)
            {
                int16_t s = -q - r;
                uint8_t family = Random(2, 5);
                Index index{q, r, s};
                Pixel pixel = IndexToPixel(index);
                elements_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius, family});
            }
        }
    }

    void Cluster::Randomize()
    {
        for(auto& [index, hexagon] : elements_)
            hexagon.family_ = Random(2, 5);
    }

    void Cluster::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < clusterRadius_ - 1;

        if(inside)
        {
            if(CheckSolution(selected))
                printf("HIT!\n");
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
            }
        }
    }

    void Cluster::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < clusterRadius_ - 1;

        if(inside)
        {
            if(CheckSolution(selected))
                printf("HIT!\n");
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
            }
        }
    }

    void Cluster::Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY) const
    {
        Index selected = PixelToIndex({cursorX, cursorY});
        bool inside = IndexDistance({0, 0, 0}, selected) < clusterRadius_ - 1;

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
                hexagon.Draw(gRenderer, palette);
            }
        }

        if(inside)
        {
            for(auto& index : topmost)
            {
                elements_.at(index).DrawHighlight(gRenderer, palette);
            }
            for(auto& index : topmost)
            {
                elements_.at(index).Draw(gRenderer, palette);
            }
        }
    }

    bool Cluster::CheckSolution(Cluster::Index index)
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

    Cluster::Index Cluster::PixelToIndex(const Cluster::Pixel& pixel) const
    {
        int16_t x = pixel.first - screenCenter_.first;
        int16_t y = pixel.second - screenCenter_.second;

        double q = (sqrt(3.0) / 3.0 * x - 1.0 / 3.0 * y) / hexRadius_;
        double r = (2.0 / 3.0 * y) / hexRadius_;

        return Round(q, r, -q - r);
    }

    Cluster::Pixel Cluster::IndexToPixel(const Cluster::Index& index) const
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        int16_t x = screenCenter_.first + (q * sqrt(3.0) + r * sqrt(3.0) / 2.0) * hexRadius_;
        int16_t y = screenCenter_.second + (3.0 / 2.0 * r) * hexRadius_;

        return {x, y};
    }

    Cluster::Index Cluster::Round(double q, double r, double s) const
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

    int16_t Cluster::IndexDistance(const Index& A, const Index& B) const
    {
        int16_t qA = std::get<0>(A);
        int16_t rA = std::get<1>(A);
        int16_t sA = std::get<2>(A);

        int16_t qB = std::get<0>(B);
        int16_t rB = std::get<1>(B);
        int16_t sB = std::get<2>(B);

        return (abs(qA - qB) + abs(rA - rB) + abs(sA - sB)) / 2;
    }

    Cluster::Index Cluster::GetClosestSelection(const Cluster::Pixel& pixel) const
    {
        Index hover = PixelToIndex(pixel);

        while(IndexDistance({0, 0, 0}, hover) >= clusterRadius_ - 1)
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

} // namespace hexvoid
