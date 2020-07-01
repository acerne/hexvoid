#include "engine.hpp"

#include <random>

uint8_t hexvoid::Random(uint8_t min, uint8_t max)
{
    static std::default_random_engine generator(time(0));
    std::uniform_int_distribution<uint8_t> distribution(min, max);
    return distribution(generator);
}

std::vector<hexvoid::Hexagon> hexvoid::GenerateHexagonGrid(int16_t gridColumns,
                                                           int16_t gridRows,
                                                           int16_t hexRadius,
                                                           int16_t spacing)
{
    std::vector<Hexagon> hexGrid;
    hexGrid.reserve(gridRows * gridColumns);

    const double cos30 = std::cos(30.0 * M_PI / 180.0);

    int16_t columnSpacing = 3 * hexRadius / 2 + spacing;
    int16_t rowSpacing = cos30 * hexRadius + spacing;

    for(int r = 0; r < gridRows; r++)
    {
        for(int c = 0; c < gridColumns; c++)
        {
            int16_t x = columnSpacing + r * columnSpacing;
            int16_t y = rowSpacing + (2 * c + r % 2) * rowSpacing;
            uint8_t colorIndex = Random(1, 4);
            hexGrid.push_back(Hexagon{x, y, hexRadius, colorIndex});
        }
    }

    return std::move(hexGrid);
}
