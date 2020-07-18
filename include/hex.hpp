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

        static void SDL(int error);
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
        Menu(uint16_t spacing);

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
        Hexagon(int16_t x, int16_t y, double radius, uint8_t family) : x_(x), y_(y), radius_(radius), family_(family) {}

        enum class Orientation
        {
            Horizontal,
            Vertical
        };

        double Distance(int16_t x, int16_t y) const;
        std::array<std::array<int16_t, 6>, 2> GetVertices(Orientation orientation, double radius) const;

        void Draw() const;
        void DrawHighlight() const;

        int16_t x_;
        int16_t y_;
        double radius_;
        uint8_t family_;

    private:
        Palette::Color GetHexagonColor() const;
    };

    class Math : public Core
    {
    public:
        typedef std::tuple<int16_t, int16_t, int16_t> Index;
        typedef std::pair<int16_t, int16_t> Pixel;

        static double RadiusToApothem(double radius);

        static Index PixelToIndex(const Pixel& pixel, double radius, Math::Pixel center);
        static Pixel IndexToPixel(const Index& index, double radius, Math::Pixel center);
        static Index RoundIndex(double q, double r, double s);
        static int16_t IndexDistance(const Index& A, const Index& B);

    private:
        Math() {}
    };

    class Grid : public Core
    {
    public:
        Grid(int16_t size, double hexRadius);
        Grid(int16_t size) : Grid(size, Engine::windowHeight_ / size / 1.5) {}

        void Randomize();
        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        std::map<Math::Index, Hexagon> elements_;
        Math::Pixel screenCenter_;
        int16_t gridSize_;
        double hexRadius_;
        int16_t gridSpacing_;

        bool CheckSolution(Math::Index index);
        void ShuffleSolution(Math::Index index);

        Math::Index GetClosestSelection(const Math::Pixel& pixel) const;
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

    class Logo : public Core
    {
    public:
        Logo(const std::string& title, Math::Pixel center, double hexRadius);

        enum class Alphabet
        {
            SEPARATOR,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z
        };

        class Symbol
        {
        public:
            Symbol(Alphabet character, Math::Pixel center, double elementRadius);
            Symbol(Alphabet character, double elementRadius) : Symbol(character, {0, 0}, elementRadius) {}

            void SetPosition(Math::Pixel center);
            void Move(Math::Pixel movement);

            Math::Pixel GetPosition() const;
            int16_t CalculateBestSpacing(const Symbol& right) const;

            void Draw() const;

        private:
            std::map<Math::Index, Hexagon> elements_;
            Math::Pixel center_;
            double elementRadius_;

            void AddHexagon(int16_t q, int16_t r);
            void GenerateH();
            void GenerateE();
            void GenerateX();
            void GenerateV();
            void GenerateO();
            void GenerateI();
            void GenerateD();
            void GenerateSeparator();
        };

        void Draw() const;

    private:
        std::vector<Symbol> logotype_;
        double elementRadius_;

        std::vector<Alphabet> ParseString(const std::string& title);
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
