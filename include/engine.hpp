#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>

#include "hexagon.hpp"

namespace hexvoid
{
    uint8_t Random(uint8_t min, uint8_t max);

    std::vector<Hexagon> GenerateHexagonGrid(int16_t gridColumns, int16_t gridRows, int16_t hexRadius, int16_t spacing);

} // namespace hexvoid
