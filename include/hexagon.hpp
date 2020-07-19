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
        static Index RotateIndex(const Index& index, int16_t rotation);
        static void ValidateIndex(const Index& index);

    private:
        Math() {}
    };

    inline Math::Index operator+(const Math::Index& A, const Math::Index& B)
    {
        return Math::Index(std::get<0>(A) + std::get<0>(B), std::get<1>(A) + std::get<1>(B),
                           std::get<2>(A) + std::get<2>(B));
    }

    inline Math::Index operator-(const Math::Index& A, const Math::Index& B)
    {
        return Math::Index(std::get<0>(A) - std::get<0>(B), std::get<1>(A) - std::get<1>(B),
                           std::get<2>(A) - std::get<2>(B));
    }

    class Tiling : public Core
    {
    public:
        Tiling() {}

        void Randomize();
        void Rotate(Math::Index rotationCenter, int16_t rotationRadius, int16_t rotation);

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
        Symbol(char character, Math::Pixel center, double elementRadius);
        Symbol(char character, double elementRadius) : Symbol(character, {0, 0}, elementRadius) {}

        void SetPosition(Math::Pixel center);
        void Move(Math::Pixel movement);

        Math::Pixel GetPosition() const;
        int16_t CalculateBestSpacing(const Symbol& right) const;

        void Draw() const;

    private:
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
        Logo(const std::string& title, Math::Pixel center, double hexRadius);

        void Draw() const;

    private:
        std::vector<Symbol> logotype_;
        double elementRadius_;
    };

} // namespace hex
