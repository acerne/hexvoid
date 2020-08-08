#pragma once

#include "hexagon.hpp"

namespace hex
{

    class FadeOut : public Background
    {
    public:
        FadeOut() {} // TODO: find a way to remove
        FadeOut(int16_t sizeX, int16_t sizeY, double hexRadius, uint8_t maxAlpha);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        RectangularGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
        uint8_t maxAlpha_;
    };

    class FadeIn : public Background
    {
    public:
        FadeIn() {} // TODO: find a way to remove
        FadeIn(int16_t sizeX, int16_t sizeY, double hexRadius, uint8_t maxAlpha);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        RectangularGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
        uint8_t maxAlpha_;
    };

    class FadeInFadeOut : public Background
    {
    public:
        FadeInFadeOut() {} // TODO: find a way to remove
        FadeInFadeOut(int16_t sizeX, int16_t sizeY, double hexRadius);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        RectangularGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
    };

    class Twinkle : public Background
    {
    public:
        Twinkle() {} // TODO: find a way to remove
        Twinkle(int16_t sizeX, int16_t sizeY, double hexRadius);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        RectangularGrid grid_;
        int8_t flashDivider = 4;
        std::map<Tiling::Index, Palette::Color> colors_;
    };

    class Snakes : public Background
    {
    public:
        Snakes() {} // TODO: find a way to remove
        Snakes(int16_t sizeX, int16_t sizeY, double hexRadius);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        RectangularGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
    };

} // namespace hex
