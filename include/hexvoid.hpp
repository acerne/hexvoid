#pragma once

#include "hex.hpp"
#include "hexagon.hpp"

namespace hexvoid
{
    class Game : public hex::Core
    {
    public:
        static void Initialize();
        static void Update();
        static void Draw();

        static void RotateClockwise(int16_t cursorX, int16_t cursorY);
        static void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        static bool CheckSolution(hex::Tiling::Index index);
        static void ShuffleSolution(hex::Tiling::Index index);

    private:
        Game() {}

        static hex::Grid gameGrid_;
    };

} // namespace hexvoid
