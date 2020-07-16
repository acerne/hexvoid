#pragma once

#include <cstddef>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_FontCache.h"

namespace hex
{
    class Core
    {
    protected:
        static FC_Font* font_;
        static SDL_Window* gWindow_;
        static SDL_Surface* gSurface_;
        static SDL_Surface* gBackground_;
        static SDL_Renderer* gRenderer_;
        static uint16_t windowWidth_;
        static uint16_t windowHeight_;
        static const char* fontPath_;

        static void Err(int error);
    };

    class Engine : public Core
    {
    public:
        static void Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight);
        static void Terminate();

        static void ChangeResolution(uint16_t windowWidth, uint16_t windowHeight);

        static void Clear();
        static void Display();

    protected:
        Engine() {}
    };

    class Menu : public Core
    {
    public:
        Menu(uint16_t spacing, int fontSize);
        ~Menu();

        void AddItem(const std::string& title, uint8_t index);
        void AddOption(const std::string& title, uint8_t index, const std::vector<std::string>& valueNames);

        int8_t Click(int16_t cursorX, int16_t cursorY);
        std::string GetSelection(const std::string& item) const;
        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        enum class MenuItemType
        {
            Item,
            Option
        };
        struct MenuItem
        {
            MenuItemType type;
            std::string title;
            uint8_t index;
            SDL_Rect box;
            std::vector<std::string> options;
            u_int8_t selected;
        };

        TTF_Font* font_;
        std::map<std::string, MenuItem> items_;
        uint16_t spacing_;

        int8_t MouseOverItemIndex(int16_t cursorX, int16_t cursorY) const;
        std::string MouseOverItemName(int16_t cursorX, int16_t cursorY) const;
        bool IsMouseInside(int16_t cursorX, int16_t cursorY, const SDL_Rect& box) const;
        void NextOption(const std::string& itemName);
    };

    class Palette : public Core
    {
    public:
        enum class Element
        {
            Background,
            Foreground,
            A,
            B,
            C,
            D
        };

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
            Color background;
            Color foreground;
            Color A;
            Color B;
            Color C;
            Color D;
        };

        static void ChangeTheme(const std::string& themeName);
        static Color GetColor(Element id);
        static std::string GetThemeName();
        static std::vector<std::string> GetThemeNames();

    private:
        Palette();

        static std::string selectedTheme_;
        static const std::map<std::string, Theme> themes_;
    };

    class Randomizer : public Core
    {
    public:
        typedef std::pair<uint8_t, uint8_t> ValueAndWeight;

        static uint8_t Random(uint8_t min, uint8_t max);
        static uint8_t WeightedRandom(const std::vector<ValueAndWeight>& candidates);

    private:
        Randomizer() {}
    };

    class Hexagon : public Core
    {
    public:
        Hexagon(int16_t x, int16_t y, int16_t radius, uint8_t family) : x_(x), y_(y), radius_(radius), family_(family)
        {}

        enum class Orientation
        {
            Horizontal,
            Vertical
        };

        double Distance(int16_t x, int16_t y) const;
        std::array<std::array<int16_t, 6>, 2> GetVertices(Orientation orientation, int16_t radius) const;

        void Draw() const;
        void DrawHighlight() const;

        int16_t x_;
        int16_t y_;
        int16_t radius_;
        uint8_t family_;

    private:
        Palette::Color GetHexagonColor() const;
    };

    class Grid : public Core
    {
    public:
        Grid(int16_t size, int16_t hexRadius);
        Grid(int16_t size) : Grid(size, Engine::windowHeight_ / size / 1.5) {}

        void Randomize();
        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        typedef std::tuple<int16_t, int16_t, int16_t> Index;
        typedef std::pair<int16_t, int16_t> Pixel;

        std::map<Index, Hexagon> elements_;
        Pixel screenCenter_;
        int16_t gridSize_;
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

    class Score : public Core
    {
    public:
        static void Start();

        static void AddScore(uint16_t points);
        static void TakeScore(uint16_t points);
        static void AddMoves(uint16_t moves);
        static void TakeMoves(uint16_t moves);
        static void RegisterMove();

        static void Draw();

    private:
        Score();
        static size_t score_;
        static size_t moves_;
        static uint16_t movesLeft_;
    };

    class Framerate : public Core
    {
    public:
        Framerate();

        void Tick();
        void Draw();

    private:
        uint16_t frameCount_;
        uint32_t lastSecond_;
        uint16_t fps_;
    };

    class SystemInfo : public Core
    {
    public:
        static void Draw();

    private:
        SystemInfo();
    };

} // namespace hex
