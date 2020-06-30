#pragma once

#include <cstddef>

namespace hexvoid
{
    class Hexagon
    {
    public:
        Hexagon() {}
        Hexagon(size_t x, size_t y, size_t diagonal, int color) : x_(x), y_(y), diagonal_(diagonal), color_(color) {}

    private:
        int color_ = -1;
        size_t x_, y_;
        size_t diagonal_;
    };
} // namespace hexvoid
