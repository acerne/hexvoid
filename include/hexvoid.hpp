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

    private:
        Game() {}

        static hex::Grid gameGrid_;
    };

} // namespace hexvoid
