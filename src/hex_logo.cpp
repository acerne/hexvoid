#include <algorithm>
#include <random>
#include <stdexcept>

#include "hexagon.hpp"

namespace hex
{

    Logo::Logo(const std::string& title, Math::Pixel center, double hexRadius)
    {
        elementRadius_ = hexRadius;
        std::vector<Symbol::Alphabet> characters = ParseString(title);

        logotype_.clear();
        for(const auto& character : characters)
            logotype_.push_back(Symbol{character, elementRadius_});

        std::vector<int16_t> spacing(logotype_.size());
        spacing.front() = 0;
        for(int i = 1; i < logotype_.size(); i++)
            spacing.at(i) = spacing.at(i - 1) + logotype_.at(i - 1).CalculateBestSpacing(logotype_.at(i));

        int16_t totalSpacing = spacing.back();
        double hexWidth = 2 * Math::RadiusToApothem(elementRadius_);
        int16_t xOffset = center.first - totalSpacing * hexWidth / 2;
        int16_t y = center.second;

        logotype_.at(0).SetPosition({xOffset, y});

        int16_t distanceFromFirstSymbol = 0;
        for(int i = 0; i < logotype_.size(); i++)
        {
            int16_t x = xOffset + spacing.at(i) * hexWidth;
            logotype_.at(i).SetPosition({x, y});
        }
    }

    std::vector<Symbol::Alphabet> Logo::ParseString(const std::string& title)
    {
        std::vector<Symbol::Alphabet> titleEnums;
        for(const char& character : title)
        {
            char C = toupper(character);
            if(C == 'A')
                titleEnums.push_back(Symbol::Alphabet::A);
            else if(C == 'B')
                titleEnums.push_back(Symbol::Alphabet::B);
            else if(C == 'C')
                titleEnums.push_back(Symbol::Alphabet::C);
            else if(C == 'D')
                titleEnums.push_back(Symbol::Alphabet::D);
            else if(C == 'E')
                titleEnums.push_back(Symbol::Alphabet::E);
            else if(C == 'F')
                titleEnums.push_back(Symbol::Alphabet::F);
            else if(C == 'G')
                titleEnums.push_back(Symbol::Alphabet::G);
            else if(C == 'H')
                titleEnums.push_back(Symbol::Alphabet::H);
            else if(C == 'I')
                titleEnums.push_back(Symbol::Alphabet::I);
            else if(C == 'J')
                titleEnums.push_back(Symbol::Alphabet::J);
            else if(C == 'K')
                titleEnums.push_back(Symbol::Alphabet::K);
            else if(C == 'L')
                titleEnums.push_back(Symbol::Alphabet::L);
            else if(C == 'M')
                titleEnums.push_back(Symbol::Alphabet::M);
            else if(C == 'N')
                titleEnums.push_back(Symbol::Alphabet::N);
            else if(C == 'O')
                titleEnums.push_back(Symbol::Alphabet::O);
            else if(C == 'P')
                titleEnums.push_back(Symbol::Alphabet::P);
            else if(C == 'Q')
                titleEnums.push_back(Symbol::Alphabet::Q);
            else if(C == 'R')
                titleEnums.push_back(Symbol::Alphabet::R);
            else if(C == 'S')
                titleEnums.push_back(Symbol::Alphabet::S);
            else if(C == 'T')
                titleEnums.push_back(Symbol::Alphabet::T);
            else if(C == 'U')
                titleEnums.push_back(Symbol::Alphabet::U);
            else if(C == 'V')
                titleEnums.push_back(Symbol::Alphabet::V);
            else if(C == 'W')
                titleEnums.push_back(Symbol::Alphabet::W);
            else if(C == 'X')
                titleEnums.push_back(Symbol::Alphabet::X);
            else if(C == 'Y')
                titleEnums.push_back(Symbol::Alphabet::Y);
            else if(C == 'Z')
                titleEnums.push_back(Symbol::Alphabet::Z);
        }
        return std::move(titleEnums);
    }

    void Logo::Draw() const
    {
        for(auto& symbol : logotype_)
            symbol.Draw();
    }

    Symbol::Symbol(Alphabet character, Math::Pixel center, double elementRadius)
    {
        tileCenter_ = center;
        hexRadius_ = elementRadius;
        switch(character)
        {
            case Alphabet::SEPARATOR:
                GenerateSeparator();
                break;
            case Alphabet::H:
                GenerateH();
                break;
            case Alphabet::E:
                GenerateE();
                break;
            case Alphabet::X:
                GenerateX();
                break;
            case Alphabet::V:
                GenerateV();
                break;
            case Alphabet::O:
                GenerateO();
                break;
            case Alphabet::I:
                GenerateI();
                break;
            case Alphabet::D:
                GenerateD();
                break;

            default:
                break;
        }
    }

    void Symbol::SetPosition(Math::Pixel center)
    {
        tileCenter_ = center;
        for(auto& element : tiles_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    void Symbol::Move(Math::Pixel movement)
    {
        tileCenter_.first += movement.first;
        tileCenter_.second += movement.second;
        for(auto& element : tiles_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, hexRadius_, tileCenter_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    Math::Pixel Symbol::GetPosition() const
    {
        return tileCenter_;
    }

    int16_t Symbol::CalculateBestSpacing(const Symbol& right) const
    {
        // TODO: take in to account diagonal spacings as well
        int16_t spacing = 10;
        int16_t closest = 10;
        for(const auto& hexA : tiles_)
        {
            int16_t qA = std::get<0>(hexA.first);
            int16_t rA = std::get<1>(hexA.first);
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
        return spacing - closest + 2;
    }

    void Symbol::Draw() const
    {
        for(auto& element : tiles_)
            element.second.Draw();
    }

    void Symbol::AddHexagon(int16_t q, int16_t r)
    {
        Math::Index index{q, r, -q - r};
        Math::Pixel pixel = Math::IndexToPixel(index, hexRadius_, tileCenter_);
        tiles_.emplace(index, Hexagon{pixel.first, pixel.second, hexRadius_, 1});
    }

    void Symbol::GenerateH()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(3, -3);
        AddHexagon(4, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(2, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(0, 3);
        AddHexagon(1, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-1, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateE()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateX()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(3, -3);
        AddHexagon(4, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-1, -1);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(2, -1);
        AddHexagon(-1, 0);
        AddHexagon(0, 0);
        AddHexagon(1, 0);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(0, 3);
        AddHexagon(1, 3);
        AddHexagon(-4, 4);
        AddHexagon(0, 4);
    }

    void Symbol::GenerateV()
    {
        AddHexagon(-1, -3);
        AddHexagon(4, -3);
        AddHexagon(-2, -2);
        AddHexagon(-1, -2);
        AddHexagon(3, -2);
        AddHexagon(4, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(1, 0);
        AddHexagon(2, 0);
        AddHexagon(-2, 1);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(1, 1);
        AddHexagon(-2, 2);
        AddHexagon(-1, 2);
        AddHexagon(0, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-2, 4);
    }

    void Symbol::GenerateO()
    {
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-3, 0);
        AddHexagon(-2, 0);
        AddHexagon(2, 0);
        AddHexagon(3, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
    }

    void Symbol::GenerateI()
    {
        AddHexagon(2, -3);
        AddHexagon(1, -3);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(0, -1);
        AddHexagon(1, -1);
        AddHexagon(0, 0);
        AddHexagon(-1, 0);
        AddHexagon(-1, 1);
        AddHexagon(0, 1);
        AddHexagon(-2, 2);
        AddHexagon(-1, 2);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
    }

    void Symbol::GenerateD()
    {
        AddHexagon(-1, -3);
        AddHexagon(0, -3);
        AddHexagon(1, -3);
        AddHexagon(2, -3);
        AddHexagon(3, -3);
        AddHexagon(-1, -2);
        AddHexagon(0, -2);
        AddHexagon(1, -2);
        AddHexagon(2, -2);
        AddHexagon(3, -2);
        AddHexagon(-2, -1);
        AddHexagon(-1, -1);
        AddHexagon(2, -1);
        AddHexagon(3, -1);
        AddHexagon(-2, 0);
        AddHexagon(-1, 0);
        AddHexagon(2, 0);
        AddHexagon(3, 0);
        AddHexagon(-3, 1);
        AddHexagon(-2, 1);
        AddHexagon(1, 1);
        AddHexagon(2, 1);
        AddHexagon(-3, 2);
        AddHexagon(-2, 2);
        AddHexagon(0, 2);
        AddHexagon(1, 2);
        AddHexagon(-4, 3);
        AddHexagon(-3, 3);
        AddHexagon(-2, 3);
        AddHexagon(-1, 3);
        AddHexagon(0, 3);
        AddHexagon(-4, 4);
        AddHexagon(-3, 4);
        AddHexagon(-2, 4);
        AddHexagon(-1, 4);
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
