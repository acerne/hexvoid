#pragma once

#include "hex.hpp"
#include "hexagon.hpp"
#include "hexbackground.hpp"

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
        static hex::HexagonalGrid gameGrid_;
        static hex::Splash titleSplash_;
        static hex::Splash gameOverSplash_;
        static hex::Snakes background_;

        static std::chrono::system_clock::time_point startTime_;
        static int64_t splashDuration_;
    };

} // namespace hexvoid
