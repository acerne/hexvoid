#pragma once

#include <chrono>

#include "hex.hpp"

namespace hex
{

    class Hexagon : public Core
    {
    public:
        Hexagon(double x, double y, double radius, uint8_t family);

        void Rotate(double angle);
        void Rotate(double pointX, double pointY, double angle);
        void RotateTo(double pointX, double pointY, double orientation);

        void Update(double x, double y);

        double Distance(double x, double y) const;
        std::array<std::array<int16_t, 6>, 2> GetVertices() const;
        std::array<std::array<int16_t, 6>, 2> CalculateVertices(double orientation, double radius) const;

        void Draw() const;
        void Draw(Palette::Color color) const;
        void DrawHighlight() const;

        uint8_t GetFamily();
        void SetFamily(uint8_t family);
        double GetX();
        double GetY();

    private:
        double x_;
        double y_;
        double radius_;
        uint8_t family_;
        double orientation_;

    private:
        std::array<std::array<int16_t, 6>, 2> vertices_;
        Palette::Color GetHexagonColor() const;
        void UpdateVertices();
    };

    class Tiling : public Core
    {
    public:
        typedef std::tuple<int16_t, int16_t, int16_t> Index;
        typedef std::pair<double, double> Pixel;

        Tiling() {}

        void Randomize();
        void Randomize(const std::vector<Index>& indices);
        bool CheckEquality(const std::vector<Index>& indices);

        void Rotate(Index rotationCenter, int16_t rotationRadius, int16_t rotation);
        void RotateWithMotion(Index rotationCenter, int16_t rotationRadius, int16_t rotationSteps);
        std::tuple<bool, Index> GetHoveringIndex(Pixel cursor);

        static double RadiusToApothem(double radius);

        static Index PixelToIndex(const Pixel& pixel, double radius, Pixel center);
        static Pixel IndexToPixel(const Index& index, double radius, Pixel center);
        static Index RoundIndex(double q, double r, double s);
        static int16_t IndexDistance(const Index& A, const Index& B);
        static Index RotateIndex(const Index& index, int16_t rotation);
        static void ValidateIndex(const Index& index);

        const std::map<Index, Hexagon>& GetTiles() const;

        void UpdatePhysics();

    protected:
        std::map<Index, Hexagon> tiles_;
        Pixel tileCenter_;
        double hexRadius_;

    private:
        struct RotatingMotion
        {
            Index rotationCenter;
            int16_t rotationSteps;
            uint16_t rotationRadius;
            double angularSpeed;
            std::chrono::system_clock::time_point lastTick;
            double curentAngle;
            double stopAngle;
        } motion_;
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

    class HexagonalGrid : public Tiling
    {
    public:
        HexagonalGrid(int16_t size, double hexRadius);
        HexagonalGrid(int16_t size) : HexagonalGrid(size, Engine::windowHeight_ / size / 1.5) {}
        HexagonalGrid() : HexagonalGrid(9, Engine::windowHeight_ / 9 / 1.5) {}

        int16_t GetGridSize();

        void Draw(int16_t cursorX, int16_t cursorY) const;

    private:
        int16_t gridSize_;

        Index GetClosestSelection(const Pixel& pixel) const;
    };

    class RectangularGrid : public Tiling
    {
    public:
        RectangularGrid(int16_t sizeX, int16_t sizeY, double hexRadius);
        RectangularGrid() : RectangularGrid(11, 7, Engine::windowHeight_ / 9 / 1.5) {}

        void Draw() const;

    private:
        int16_t gridSizeX_;
        int16_t gridSizeY_;
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

    class Splash : public Core
    {
    public:
        Splash() {} // TODO: find a way to remove
        Splash(const std::string& title, Tiling::Pixel center, double hexRadius);

        void Draw() const;

    private:
        std::vector<Symbol> logotype_;
        double elementRadius_;
    };

    class FadeOut : public Background
    {
    public:
        FadeOut() {} // TODO: find a way to remove
        FadeOut(int16_t size, double hexRadius, uint8_t maxAlpha);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        HexagonalGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
        uint8_t maxAlpha_;
    };

    class FadeIn : public Background
    {
    public:
        FadeIn() {} // TODO: find a way to remove
        FadeIn(int16_t size, double hexRadius, uint8_t maxAlpha);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        HexagonalGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
        uint8_t maxAlpha_;
    };

    class FadeInFadeOut : public Background
    {
    public:
        FadeInFadeOut() {} // TODO: find a way to remove
        FadeInFadeOut(int16_t size, double hexRadius);

        void Initialize();

        void UpdatePhysics();
        void Draw();

    private:
        HexagonalGrid grid_;
        std::map<Tiling::Index, Palette::Color> colors_;
    };

} // namespace hex
