#include <algorithm>
#include <random>
#include <stdexcept>

#include "hexagon.hpp"

namespace hex
{

    Symbol::Symbol(char character, Tiling::Pixel center, double elementRadius)
    {
        tileCenter_ = center;
        hexRadius_ = elementRadius;
        if(character == 'A')
            GenerateA();
        else if(character == 'B')
            GenerateB();
        else if(character == 'C')
            GenerateC();
        else if(character == 'D')
            GenerateD();
        else if(character == 'E')
            GenerateE();
        else if(character == 'F')
            GenerateF();
        else if(character == 'G')
            GenerateG();
        else if(character == 'H')
            GenerateH();
        else if(character == 'I')
            GenerateI();
        else if(character == 'J')
            GenerateJ();
        else if(character == 'K')
            GenerateK();
        else if(character == 'L')
            GenerateL();
        else if(character == 'M')
            GenerateM();
        else if(character == 'N')
            GenerateN();
        else if(character == 'O')
            GenerateO();
        else if(character == 'P')
            GenerateP();
        else if(character == 'Q')
            GenerateQ();
        else if(character == 'R')
            GenerateR();
        else if(character == 'S')
            GenerateS();
        else if(character == 'T')
            GenerateT();
        else if(character == 'U')
            GenerateU();
        else if(character == 'V')
            GenerateV();
        else if(character == 'W')
            GenerateW();
        else if(character == 'X')
            GenerateX();
        else if(character == 'Y')
            GenerateY();
        else if(character == 'Z')
            GenerateZ();
        else if(character == ' ')
            isSpace = true;
    }

    void Symbol::SetPosition(Tiling::Pixel center)
    {
        tileCenter_ = center;
        for(auto& element : tiles_)
        {
            Pixel pixel = IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    void Symbol::Move(Tiling::Pixel movement)
    {
        tileCenter_.first += movement.first;
        tileCenter_.second += movement.second;
        for(auto& element : tiles_)
        {
            Pixel pixel = IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    Tiling::Pixel Symbol::GetPosition() const
    {
        return tileCenter_;
    }

    int16_t Symbol::CalculateBestSpacing(const Symbol& right) const
    {
        // TODO: take in to account diagonal spacings as well - is it really needed?
        int16_t spacing = 10;
        int16_t closest = 10;
        for(const auto& hexA : tiles_)
        {
            int16_t qA = std::get<0>(hexA.first);
            int16_t rA = std::get<1>(hexA.first);
            if(right.isSpace)
            {
                int16_t apart = spacing - qA;
                if(apart < closest) closest = apart;
            }
            else
            {
                for(const auto& hexB : right.tiles_)
                {
                    int16_t rB = std::get<1>(hexB.first);
                    if(rA == rB)
                    {
                        int16_t qB = std::get<0>(hexB.first);
                        int16_t apart = qB + spacing - qA;
                        if(apart < closest) closest = apart;
                    }
                }
            }
        }
        return spacing - closest + 2;
    }

    void Symbol::Draw() const
    {
        for(auto& element : tiles_)
            element.second.Draw();
    }

    void Symbol::AddHexagon(int16_t q, int16_t r)
    {
        Index index{q, r, -q - r};
        Pixel pixel = IndexToPixel(index, hexRadius_, tileCenter_);
        tiles_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius_, 1});
    }

    void Symbol::AddHexagonRow(const std::vector<int16_t>& qList, int16_t r)
    {
        for(const auto& q : qList)
            AddHexagon(q, r);
    }

    void Symbol::AddHexagonRow(int16_t qStart, int16_t qEnd, int16_t r)
    {
        for(int16_t q = qStart; q <= qEnd; q++)
            AddHexagon(q, r);
    }

    void Symbol::GenerateA()
    {
        AddHexagonRow({1, 2}, -3);
        AddHexagonRow(0, 2, -2);
        AddHexagonRow(-1, 2, -1);
        AddHexagonRow({-2, -1, 1, 2}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow(-3, 1, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, 0}, 4);
    }

    void Symbol::GenerateB()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow(-2, 2, 0);
        AddHexagonRow(-3, 2, 1);
        AddHexagonRow({-3, -2, 1, 2}, 2);
        AddHexagonRow(-4, 1, 3);
        AddHexagonRow(-4, 0, 4);
    }

    void Symbol::GenerateC()
    {
        AddHexagonRow(0, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow({-3, -2}, 0);
        AddHexagonRow({-3, -2}, 1);
        AddHexagonRow({-3, -2}, 2);
        AddHexagonRow(-3, 0, 3);
        AddHexagonRow(-3, -1, 4);
    }

    void Symbol::GenerateD()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-2, -1, 2, 3}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow(-4, 0, 3);
        AddHexagonRow(-4, -1, 4);
    }

    void Symbol::GenerateE()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow(-2, 1, 0);
        AddHexagonRow(-3, 1, 1);
        AddHexagonRow({-3, -2}, 2);
        AddHexagonRow(-4, 0, 3);
        AddHexagonRow(-4, 0, 4);
    }

    void Symbol::GenerateF()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow(-2, 1, 0);
        AddHexagonRow(-3, 1, 1);
        AddHexagonRow({-3, -2}, 2);
        AddHexagonRow({-4, -3}, 3);
        AddHexagonRow({-4, -3}, 4);
    }

    void Symbol::GenerateG()
    {
        AddHexagonRow(0, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow({-3, -2}, 0);
        AddHexagonRow({-3, -2, 0, 1}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow(-3, 0, 3);
        AddHexagonRow(-3, -1, 4);
    }

    void Symbol::GenerateH()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow(-2, 2, 0);
        AddHexagonRow(-3, 2, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, -3, -1, 0}, 4);
    }

    void Symbol::GenerateI()
    {
        AddHexagonRow({2, 1}, -3);
        AddHexagonRow({1, 2}, -2);
        AddHexagonRow({0, 1}, -1);
        AddHexagonRow({0, 1}, 0);
        AddHexagonRow({-1, 0}, 1);
        AddHexagonRow({-1, 0}, 2);
        AddHexagonRow({-2, -1}, 3);
        AddHexagonRow({-2, -1}, 4);
    }

    void Symbol::GenerateJ()
    {
        AddHexagonRow({2, 1}, -3);
        AddHexagonRow({1, 2}, -2);
        AddHexagonRow({0, 1}, -1);
        AddHexagonRow({0, 1}, 0);
        AddHexagonRow({-1, 0}, 1);
        AddHexagonRow({-4, -3, -1, 0}, 2);
        AddHexagonRow(-4, -1, 3);
        AddHexagonRow(-4, -2, 4);
    }

    void Symbol::GenerateK()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow({-2, -1, 1, 2}, -1);
        AddHexagonRow(-2, 1, 0);
        AddHexagonRow(-3, 0, 1);
        AddHexagonRow(-3, 0, 2);
        AddHexagonRow({-4, -3, -1, 0}, 3);
        AddHexagonRow({-4, -3, -1, 0}, 4);
    }

    void Symbol::GenerateL()
    {
        AddHexagonRow({-1, 0}, -3);
        AddHexagonRow({-1, 0}, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow({-2, -1}, 0);
        AddHexagonRow({-3, -2}, 1);
        AddHexagonRow({-3, -2}, 2);
        AddHexagonRow(-4, 0, 3);
        AddHexagonRow(-4, -1, 4);
    }

    void Symbol::GenerateM()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow(-2, 3, -1);
        AddHexagonRow({-2, 0, 2}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, 0}, 4);
    }

    void Symbol::GenerateN()
    {

        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow({-2, -1, 0, 2, 3}, -1);
        AddHexagonRow({-2, -1, 0, 2}, 0);
        AddHexagonRow({-3, -2, 0, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, 0}, 4);
    }

    void Symbol::GenerateO()
    {
        AddHexagonRow(0, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-3, -2, 2, 3}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow(-3, 0, 3);
        AddHexagonRow(-3, -1, 4);
    }

    void Symbol::GenerateP()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-2, 1, 2}, 0);
        AddHexagonRow(-3, 1, 1);
        AddHexagonRow({-3, -2}, 2);
        AddHexagonRow({-4, -3}, 3);
        AddHexagonRow({-4, -3}, 4);
    }

    void Symbol::GenerateQ()
    {
        AddHexagonRow(0, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-3, -2, 2, 3}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow(-3, 0, 3);
        AddHexagonRow(-3, 0, 4);
    }

    void Symbol::GenerateR()
    {
        AddHexagonRow(-1, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-2, 1, 2}, 0);
        AddHexagonRow(-3, 1, 1);
        AddHexagonRow(-3, 0, 2);
        AddHexagonRow({-4, -3, -1, 0}, 3);
        AddHexagonRow({-4, -3, -1, 0}, 4);
    }

    void Symbol::GenerateS()
    {
        AddHexagonRow(0, 3, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({-2, -1}, -1);
        AddHexagonRow(-2, 1, 0);
        AddHexagonRow(-2, 1, 1);
        AddHexagonRow({0, 1}, 2);
        AddHexagonRow(-4, 0, 3);
        AddHexagonRow(-4, -1, 4);
    }

    void Symbol::GenerateT()
    {
        AddHexagonRow(-1, 4, -3);
        AddHexagonRow(-1, 4, -2);
        AddHexagonRow({0, 1}, -1);
        AddHexagonRow({0, 1}, 0);
        AddHexagonRow({-1, 0}, 1);
        AddHexagonRow({-1, 0}, 2);
        AddHexagonRow({-2, -1}, 3);
        AddHexagonRow({-2, -1}, 4);
    }

    void Symbol::GenerateU()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-2, -1, 1, 2}, 0);
        AddHexagonRow({-3, -2, 1, 2}, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow(-3, 0, 3);
        AddHexagonRow(-3, -1, 4);
    }

    void Symbol::GenerateV()
    {
        AddHexagonRow({-1, 4}, -3);
        AddHexagonRow({-2, -1, 3, 4}, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-2, -1, 1, 2}, 0);
        AddHexagonRow(-2, 1, 1);
        AddHexagonRow(-2, 0, 2);
        AddHexagonRow({-2, -1}, 3);
        AddHexagon(-2, 4);
    }

    void Symbol::GenerateW()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-2, -1, 3, 4}, -2);
        AddHexagonRow({-2, -1, 2, 3}, -1);
        AddHexagonRow({-3, -2, 0, 2, 3}, 0);
        AddHexagonRow({-3, -1, 0, 2}, 1);
        AddHexagonRow(-4, 2, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, 0}, 4);
    }

    void Symbol::GenerateX()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow(-1, 2, -1);
        AddHexagonRow(-1, 1, 0);
        AddHexagonRow(-2, 1, 1);
        AddHexagonRow({-3, -2, 0, 1}, 2);
        AddHexagonRow({-4, -3, 0, 1}, 3);
        AddHexagonRow({-4, 0}, 4);
    }

    void Symbol::GenerateY()
    {
        AddHexagonRow({-1, 0, 3, 4}, -3);
        AddHexagonRow({-1, 0, 2, 3}, -2);
        AddHexagonRow(-1, 2, -1);
        AddHexagonRow(-1, 1, 0);
        AddHexagonRow({-1, 0}, 1);
        AddHexagonRow({-2, -1}, 2);
        AddHexagonRow({-3, -2}, 3);
        AddHexagonRow({-4, -3}, 4);
    }

    void Symbol::GenerateZ()
    {
        AddHexagonRow(-1, 4, -3);
        AddHexagonRow(-1, 3, -2);
        AddHexagonRow({1, 2}, -1);
        AddHexagonRow({0, 1}, 0);
        AddHexagonRow({-1, 0}, 1);
        AddHexagonRow({-2, -1}, 2);
        AddHexagonRow(-3, 1, 3);
        AddHexagonRow(-4, 0, 4);
    }

    void Symbol::GenerateSeparator()
    {
        AddHexagon(2, -4);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(1, -2);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(0, 0);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(-1, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-2, 4);
    }

} // namespace hex
