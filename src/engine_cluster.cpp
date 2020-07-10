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

    Cluster::Cluster(int16_t radius, int16_t hexRadius, int16_t spacing)
    {
        clusterRadius_ = radius;
        hexRadius_ = hexRadius;
        clusterSpacing_ = spacing;
        clusterSize_ = 2 * radius - 1;
        for(int i = 1; i < radius; i++)
            clusterSize_ += 2 * (2 * radius - 1 - i);

        elements_.clear();
        elements_.reserve(clusterSize_);

        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t columnSpacing = 3 * hexRadius / 2 + spacing;
        int16_t rowSpacing = cos30 * hexRadius + spacing;

        int16_t screenCenterX = WINDOW_WIDTH / 2;
        int16_t screenCenterY = WINDOW_HEIGHT / 2;

        for(int r = -radius + 1; r < radius; r++)
        {
            int columns;
            if(r < 0)
                columns = 2 * radius + r - 1;
            else
                columns = 2 * radius - r - 1;

            int columnStart = (2 * radius - columns) / 2 - radius + 1;

            for(int c = columnStart; c < columnStart + columns; c++)
            {
                int16_t x = screenCenterX + (2 * c - abs(r) % 2) * rowSpacing;
                int16_t y = screenCenterY + r * columnSpacing;
                uint8_t family = Random(2, 5);
                elements_.emplace_back(x, y, hexRadius, family);
            }
        }
    }

    void Cluster::Randomize()
    {
        for(auto& hex : elements_)
            hex.family_ = Random(2, 5);
    }

    void Cluster::Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY)
    {
        int16_t closest = FindClosestElement(cursorX, cursorY);
        for(int i = 0; i < elements_.size(); i++)
        {
            if(i == closest)
            {
                elements_.at(i).DrawBackground(gRenderer, palette);
                elements_.at(i).Draw(gRenderer, palette);
            }
            else if(elements_.at(i).Distance(elements_.at(closest).x_, elements_.at(closest).y_)
                    < 2 * elements_.at(closest).radius_)
            {
                elements_.at(i).DrawBackground(gRenderer, palette);
                elements_.at(i).Draw(gRenderer, palette);
            }
            else
            {
                elements_.at(i).Draw(gRenderer, palette);
            }
        }
    }

    uint16_t Cluster::FindClosestElement(int16_t x, int16_t y)
    {
        double minDistance = std::numeric_limits<double>::infinity();
        uint16_t bestIndex = -1;
        for(int i = 0; i < elements_.size(); i++)
        {
            double distance = elements_.at(i).Distance(x, y);
            if(distance < minDistance)
            {
                minDistance = distance;
                bestIndex = i;
            }
        }
        return bestIndex;
    }

} // namespace hexvoid
