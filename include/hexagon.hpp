#pragma once

#include <cstddef>
#include <stdint.h>

namespace hexvoid
{
    struct Hexagon
    {
        int16_t x;
        int16_t y;
        int16_t radius;
        uint8_t color;
    };
} // namespace hexvoid
