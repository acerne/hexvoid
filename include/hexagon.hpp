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

        void Update(int16_t x, int16_t y);

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

    class Tiling : public Core
    {
    public:
        typedef std::tuple<int16_t, int16_t, int16_t> Index;
        typedef std::pair<int16_t, int16_t> Pixel;

        Tiling() {}

        void Randomize();
        void Rotate(Index rotationCenter, int16_t rotationRadius, int16_t rotation);

        static double RadiusToApothem(double radius);

        static Index PixelToIndex(const Pixel& pixel, double radius, Pixel center);
        static Pixel IndexToPixel(const Index& index, double radius, Pixel center);
        static Index RoundIndex(double q, double r, double s);
        static int16_t IndexDistance(const Index& A, const Index& B);
        static Index RotateIndex(const Index& index, int16_t rotation);
        static void ValidateIndex(const Index& index);

    protected:
        std::map<Index, Hexagon> tiles_;
        Pixel tileCenter_;
        double hexRadius_;

    private:
    };

    inline Tiling::Index operator+(const Tiling::Index& A, const Tiling::Index& B)
    {
        return Tiling::Index(std::get<0>(A) + std::get<0>(B), std::get<1>(A) + std::get<1>(B),
                             std::get<2>(A) + std::get<2>(B));
    }

    inline Tiling::Index operator-(const Tiling::Index& A, const Tiling::Index& B)
    {
        return Tiling::Index(std::get<0>(A) - std::get<0>(B), std::get<1>(A) - std::get<1>(B),
                             std::get<2>(A) - std::get<2>(B));
    }

    class Grid : public Tiling
    {
    public:
        Grid(int16_t size, double hexRadius);
        Grid(int16_t size) : Grid(size, Engine::windowHeight_ / size / 1.5) {}
        Grid() : Grid(9, Engine::windowHeight_ / 9 / 1.5) {}

        void RotateClockwise(int16_t cursorX, int16_t cursorY);
        void RotateCounterClockwise(int16_t cursorX, int16_t cursorY);

        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        int16_t gridSize_;

        bool CheckSolution(Index index);
        void ShuffleSolution(Index index);

        Index GetClosestSelection(const Pixel& pixel) const;
    };

    class Symbol : public Tiling
    {
    public:
        Symbol(char character, Pixel center, double elementRadius);
        Symbol(char character, double elementRadius) : Symbol(character, {0, 0}, elementRadius) {}

        void SetPosition(Pixel center);
        void Move(Pixel movement);

        Pixel GetPosition() const;
        int16_t CalculateBestSpacing(const Symbol& right) const;

        void Draw() const;

    private:
        bool isSpace = false;

        void AddHexagon(int16_t q, int16_t r);
        void AddHexagonRow(const std::vector<int16_t>& qList, int16_t r);
        void AddHexagonRow(int16_t qStart, int16_t qEnd, int16_t r);
        void GenerateA();
        void GenerateB();
        void GenerateC();
        void GenerateD();
        void GenerateE();
        void GenerateF();
        void GenerateG();
        void GenerateH();
        void GenerateI();
        void GenerateJ();
        void GenerateK();
        void GenerateL();
        void GenerateM();
        void GenerateN();
        void GenerateO();
        void GenerateP();
        void GenerateQ();
        void GenerateR();
        void GenerateS();
        void GenerateT();
        void GenerateU();
        void GenerateV();
        void GenerateW();
        void GenerateX();
        void GenerateY();
        void GenerateZ();
        void GenerateSeparator();
    };

    class Logo : public Core
    {
    public:
        Logo(const std::string& title, Tiling::Pixel center, double hexRadius);

        void Draw() const;

    private:
        std::vector<Symbol> logotype_;
        double elementRadius_;
    };

} // namespace hex
