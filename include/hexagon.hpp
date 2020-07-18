#pragma once

#include "hex.hpp"

namespace hex
{

    class Hexagon : public Core
    {
    public:
        enum class Orientation
        {
            Horizontal,
            Vertical
        };

        Hexagon(int16_t x, int16_t y, double radius, uint8_t family) : x_(x), y_(y), radius_(radius), family_(family) {}

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

    class Tiling : public Core
    {
    public:
        Tiling() {}

    protected:
        std::map<Math::Index, Hexagon> tiles_;
        Math::Pixel tileCenter_;
        double hexRadius_;

    private:
    };

    class Grid : public Tiling
    {
    public:
        Grid(int16_t size, double hexRadius);
        Grid(int16_t size) : Grid(size, Engine::windowHeight_ / size / 1.5) {}

        void Randomize();
        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        int16_t gridSize_;

        bool CheckSolution(Math::Index index);
        void ShuffleSolution(Math::Index index);

        Math::Index GetClosestSelection(const Math::Pixel& pixel) const;
    };

    class Symbol : public Tiling
    {
    public:
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

        Symbol(Alphabet character, Math::Pixel center, double elementRadius);
        Symbol(Alphabet character, double elementRadius) : Symbol(character, {0, 0}, elementRadius) {}

        void SetPosition(Math::Pixel center);
        void Move(Math::Pixel movement);

        Math::Pixel GetPosition() const;
        int16_t CalculateBestSpacing(const Symbol& right) const;

        void Draw() const;

    private:
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

    class Logo : public Core
    {
    public:
        Logo(const std::string& title, Math::Pixel center, double hexRadius);

        void Draw() const;

    private:
        std::vector<Symbol> logotype_;
        double elementRadius_;

        std::vector<Symbol::Alphabet> ParseString(const std::string& title);
    };

} // namespace hex
