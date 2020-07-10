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

        for(int r = -radius + 1; r < radius; r++)
        {
            int columns;
            if(r < 0)
                columns = 2 * radius + r - 1;
            else
                columns = 2 * radius - r - 1;

            std::vector<Hexagon> row;
            row.reserve(columns);

            int columnStart = (2 * radius - columns) / 2 - radius + 1;

            for(int c = columnStart; c < columnStart + columns; c++)
            {
                int16_t x = screenCenterX + (2 * c - abs(r) % 2) * rowSpacing;
                int16_t y = screenCenterY + r * columnSpacing;
                uint8_t family = Random(2, 5);
                row.emplace_back(x, y, hexRadius, family);
            }
            elements_.push_back(std::move(row));
            rowOffset_.push_back(columnStart + radius - 1);
        }
    }

    void Cluster::Randomize()
    {
        for(auto& row : elements_)
            for(auto& hex : row)
                hex.family_ = Random(2, 5);
    }

    void Cluster::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        int16_t r, c;
        FindClosestInnerElement(cursorX, cursorY, r, c);

        uint8_t swap = elements_.at(r).at(c + 1).family_;
        elements_.at(r).at(c + 1).family_ = elements_.at(r - 1).at(c).family_;
        elements_.at(r - 1).at(c).family_ = elements_.at(r - 1).at(c - 1).family_;
        elements_.at(r - 1).at(c - 1).family_ = elements_.at(r).at(c - 1).family_;
        elements_.at(r).at(c - 1).family_ = elements_.at(r + 1).at(c - 1).family_;
        elements_.at(r + 1).at(c - 1).family_ = elements_.at(r + 1).at(c).family_;
        elements_.at(r + 1).at(c).family_ = swap;
    }

    void Cluster::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        int16_t r, c;
        FindClosestInnerElement(cursorX, cursorY, r, c);

        uint8_t swap = elements_.at(r).at(c + 1).family_;
        elements_.at(r).at(c + 1).family_ = elements_.at(r + 1).at(c).family_;
        elements_.at(r + 1).at(c).family_ = elements_.at(r + 1).at(c - 1).family_;
        elements_.at(r + 1).at(c - 1).family_ = elements_.at(r).at(c - 1).family_;
        elements_.at(r).at(c - 1).family_ = elements_.at(r - 1).at(c - 1).family_;
        elements_.at(r - 1).at(c - 1).family_ = elements_.at(r - 1).at(c).family_;
        elements_.at(r - 1).at(c).family_ = swap;
    }

    void Cluster::Draw(SDL_Renderer*& gRenderer, const Palette& palette, int16_t cursorX, int16_t cursorY)
    {
        int16_t closestRow, closestColumn;
        FindClosestInnerElement(cursorX, cursorY, closestRow, closestColumn);

        for(int r = 0; r < elements_.size(); r++)
        {
            for(int c = 0; c < elements_.at(r).size(); c++)
            {
                if(r == closestRow && c == closestColumn)
                {
                    elements_.at(r).at(c).DrawBackground(gRenderer, palette);
                    elements_.at(r).at(c).Draw(gRenderer, palette);
                }
                else if(elements_.at(r).at(c).Distance(elements_.at(closestRow).at(closestColumn).x_,
                                                       elements_.at(closestRow).at(closestColumn).y_)
                        < 2 * elements_.at(closestRow).at(closestColumn).radius_)
                {
                    elements_.at(r).at(c).DrawBackground(gRenderer, palette);
                    elements_.at(r).at(c).Draw(gRenderer, palette);
                }
                else
                {
                    elements_.at(r).at(c).Draw(gRenderer, palette);
                }
            }
        }
    }

    void Cluster::FindClosestInnerElement(int16_t x, int16_t y, int16_t& row, int16_t& column)
    {
        double minDistance = std::numeric_limits<double>::infinity();
        row = -1;
        column = -1;
        for(int r = 1; r < elements_.size() - 1; r++)
        {
            for(int c = 1; c < elements_.at(r).size() - 1; c++)
            {
                double distance = elements_.at(r).at(c).Distance(x, y);
                if(distance < minDistance)
                {
                    minDistance = distance;
                    row = r;
                    column = c;
                }
            }
        }
    }

} // namespace hexvoid
