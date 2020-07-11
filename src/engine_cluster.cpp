#include <algorithm>
#include <iostream> // DELETE
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

    Cluster::Cluster(int16_t radius, int16_t hexRadius, int16_t spacing)
    {
        clusterRadius_ = radius;
        hexRadius_ = hexRadius;
        clusterSpacing_ = spacing;
        clusterSize_ = 2 * radius - 1;
        for(int i = 1; i < radius; i++)
            clusterSize_ += 2 * (2 * radius - 1 - i);

        elements_.clear();

        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t columnSpacing = 3 * hexRadius / 2 + spacing;
        int16_t rowSpacing = cos30 * hexRadius + spacing;

        int16_t screenCenterX = WINDOW_WIDTH / 2;
        int16_t screenCenterY = WINDOW_HEIGHT / 2;

        for(int16_t r = -radius + 1; r < radius; r++)
        {
            int16_t columns;
            if(r < 0)
                columns = 2 * radius + r - 1;
            else
                columns = 2 * radius - r - 1;

            int16_t columnStart = (2 * radius - columns) / 2 - radius + 1;

            for(int16_t c = columnStart; c < columnStart + columns; c++)
            {
                int16_t x = screenCenterX + (2 * c - abs(r) % 2) * rowSpacing;
                int16_t y = screenCenterY + r * columnSpacing;
                uint8_t family = Random(2, 5);
                std::pair<int16_t, int16_t> location{r, c};
                elements_.emplace(location, Hexagon{x, y, hexRadius, family});
            }
        }
    } // namespace hexvoid

    void Cluster::Randomize()
    {
        for(auto& [index, hexagon] : elements_)
            hexagon.family_ = Random(2, 5);
    }

    void Cluster::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        int16_t r, c;
        auto selected = Match(cursorX, cursorY);

        // uint8_t swap = elements_.at(r).at(c + 1).family_;
        // elements_.at(r).at(c + 1).family_ = elements_.at(r - 1).at(c).family_;
        // elements_.at(r - 1).at(c).family_ = elements_.at(r - 1).at(c - 1).family_;
        // elements_.at(r - 1).at(c - 1).family_ = elements_.at(r).at(c - 1).family_;
        // elements_.at(r).at(c - 1).family_ = elements_.at(r + 1).at(c - 1).family_;
        // elements_.at(r + 1).at(c - 1).family_ = elements_.at(r + 1).at(c).family_;
        // elements_.at(r + 1).at(c).family_ = swap;
    }

    void Cluster::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        int16_t r, c;
        auto selected = Match(cursorX, cursorY);

        // uint8_t swap = elements_.at(r).at(c + 1).family_;
        // elements_.at(r).at(c + 1).family_ = elements_.at(r + 1).at(c).family_;
        // elements_.at(r + 1).at(c).family_ = elements_.at(r + 1).at(c - 1).family_;
        // elements_.at(r + 1).at(c - 1).family_ = elements_.at(r).at(c - 1).family_;
        // elements_.at(r).at(c - 1).family_ = elements_.at(r - 1).at(c - 1).family_;
        // elements_.at(r - 1).at(c - 1).family_ = elements_.at(r - 1).at(c).family_;
        // elements_.at(r - 1).at(c).family_ = swap;
    }

    void Cluster::Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY) const
    {
        int16_t closestRow, closestColumn;
        auto selected = Match(cursorX, cursorY);

        for(const auto& [index, hexagon] : elements_)
        {
            if(UnitDistance(index, selected) <= 2)
            {
                hexagon.DrawBackground(gRenderer, palette);
                hexagon.Draw(gRenderer, palette);
            }
            else
            {
                hexagon.Draw(gRenderer, palette);
            }
        }
    }

    std::pair<int16_t, int16_t> Cluster::Match(int16_t x, int16_t y) const
    {
        double minDistance = std::numeric_limits<double>::infinity();
        std::pair<int16_t, int16_t> best{0, 0};

        int16_t rowStart = -clusterRadius_ + 2;
        int16_t rowEnd = clusterRadius_ - 2;
        for(int16_t r = rowStart; r <= rowEnd; r++)
        {
            int16_t columnStart = -clusterRadius_ + 2 + ceil(abs(r) / 2.0);
            int16_t columnEnd = clusterRadius_ - 2 - floor(abs(r) / 2.0);
            for(int16_t c = columnStart; c <= columnEnd; c++)
            {
                double distance = elements_.at({r, c}).Distance(x, y);
                if(distance < minDistance)
                {
                    minDistance = distance;
                    best.first = r;
                    best.second = c;
                }
            }
        }
        return std::move(best);
    }

} // namespace hexvoid
