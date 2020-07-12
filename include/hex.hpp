#pragma once

#include <cstddef>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace hex
{
    class Engine
    {
    public:
        static void EnableDebug();

        static void Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight);
        static void Terminate();

        static void Clear();
        static void Display();

    protected:
        Engine() {}
        static bool debug_;
        static SDL_Window* gWindow_;
        static SDL_Surface* gSurface_;
        static SDL_Surface* gBackground_;
        static SDL_Renderer* gRenderer_;
        static uint16_t windowWidth_;
        static uint16_t windowHeight_;
    };

    class Text : public Engine
    {
    public:
        Text(int fontSize, uint8_t lines = 1);
        ~Text();

        void Place(int16_t x, int16_t y);
        void Set(const std::string& text, uint8_t line = 0);

        void Draw() const;

    private:
        TTF_Font* font_;
        std::vector<std::string> text_;
        uint8_t lines_;
        int16_t x_;
        int16_t y_;
    };

    class Palette : public Engine
    {
    public:
        Palette();

        struct Color
        {
            Color(uint8_t r, uint8_t g, uint8_t b) {}
            Color(int32_t hexValue)
            {
                r = ((hexValue >> 16) & 0xFF);
                g = ((hexValue >> 8) & 0xFF);
                b = ((hexValue)&0xFF);
            }
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        struct Theme
        {
            std::string name;
            Color background;
            Color foreground;
            Color A;
            Color B;
            Color C;
            Color D;
        };

        Color GetThemeColor(int index) const;
        std::string GetThemeName() const;

        void NextTheme();
        void PreviousTheme();

        void DrawInfo();

    private:
        uint8_t selectedTheme_;
        std::vector<Theme> themeList_;
        Text text_ = Text(26);
    };

    class Hexagon : public Engine
    {
    public:
        Hexagon(int16_t x, int16_t y, int16_t radius, uint8_t family);

        double Distance(int16_t x, int16_t y) const;

        void Draw(const Palette& palette) const;
        void DrawHighlight(const Palette& palette) const;

        int16_t x_;
        int16_t y_;
        int16_t radius_;
        uint8_t family_;
    };

    class Grid : public Engine
    {
    public:
        Grid(int16_t radius, int16_t hexRadius);

        void Randomize();
        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(const Palette& palette, int16_t cursorX, int16_t cursorY) const;

    private:
        typedef std::tuple<int16_t, int16_t, int16_t> Index;
        typedef std::pair<int16_t, int16_t> Pixel;

        std::map<Index, Hexagon> elements_;
        Pixel screenCenter_;
        int16_t gridRadius_;
        int16_t hexRadius_;
        int16_t gridSpacing_;

        bool CheckSolution(Index index);
        void ShuffleSolution(Index index);

        Index PixelToIndex(const Pixel& pixel) const;
        Pixel IndexToPixel(const Index& index) const;
        Index Round(double q, double r, double s) const;
        int16_t IndexDistance(const Index& A, const Index& B) const;
        Index GetClosestSelection(const Pixel& pixel) const;
    };

    class Framerate : public Engine
    {
    public:
        Framerate();

        void Tick();
        void Draw();

    private:
        uint16_t frameCount_;
        uint32_t lastSecond_;
        uint16_t fps_;
        Text text_ = Text(16);
    };

    class SystemInfo : public Engine
    {
    public:
        SystemInfo();

        void Draw();

    private:
        Text textInfo_ = Text(12, 8);
    };

} // namespace hex
