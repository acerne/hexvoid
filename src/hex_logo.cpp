#include <algorithm>
#include <random>
#include <stdexcept>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "hex.hpp"

namespace hex
{

    Logo::Logo(const std::string& title, Math::Pixel center, double hexRadius)
    {
        elementRadius_ = hexRadius;
        std::vector<Alphabet> characters = ParseString(title);

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

    std::vector<Logo::Alphabet> Logo::ParseString(const std::string& title)
    {
        std::vector<Alphabet> titleEnums;
        for(const char& character : title)
        {
            char C = toupper(character);
            if(C == 'A')
                titleEnums.push_back(Alphabet::A);
            else if(C == 'B')
                titleEnums.push_back(Alphabet::B);
            else if(C == 'C')
                titleEnums.push_back(Alphabet::C);
            else if(C == 'D')
                titleEnums.push_back(Alphabet::D);
            else if(C == 'E')
                titleEnums.push_back(Alphabet::E);
            else if(C == 'F')
                titleEnums.push_back(Alphabet::F);
            else if(C == 'G')
                titleEnums.push_back(Alphabet::G);
            else if(C == 'H')
                titleEnums.push_back(Alphabet::H);
            else if(C == 'I')
                titleEnums.push_back(Alphabet::I);
            else if(C == 'J')
                titleEnums.push_back(Alphabet::J);
            else if(C == 'K')
                titleEnums.push_back(Alphabet::K);
            else if(C == 'L')
                titleEnums.push_back(Alphabet::L);
            else if(C == 'M')
                titleEnums.push_back(Alphabet::M);
            else if(C == 'N')
                titleEnums.push_back(Alphabet::N);
            else if(C == 'O')
                titleEnums.push_back(Alphabet::O);
            else if(C == 'P')
                titleEnums.push_back(Alphabet::P);
            else if(C == 'Q')
                titleEnums.push_back(Alphabet::Q);
            else if(C == 'R')
                titleEnums.push_back(Alphabet::R);
            else if(C == 'S')
                titleEnums.push_back(Alphabet::S);
            else if(C == 'T')
                titleEnums.push_back(Alphabet::T);
            else if(C == 'U')
                titleEnums.push_back(Alphabet::U);
            else if(C == 'V')
                titleEnums.push_back(Alphabet::V);
            else if(C == 'W')
                titleEnums.push_back(Alphabet::W);
            else if(C == 'X')
                titleEnums.push_back(Alphabet::X);
            else if(C == 'Y')
                titleEnums.push_back(Alphabet::Y);
            else if(C == 'Z')
                titleEnums.push_back(Alphabet::Z);
        }
        return std::move(titleEnums);
    }

    void Logo::Draw() const
    {
        for(auto& symbol : logotype_)
            symbol.Draw();
    }

    Logo::Symbol::Symbol(Alphabet character, Math::Pixel center, double elementRadius)
    {
        center_ = center;
        elementRadius_ = elementRadius;
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

    void Logo::Symbol::SetPosition(Math::Pixel center)
    {
        center_ = center;
        for(auto& element : elements_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, elementRadius_, center_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    void Logo::Symbol::Move(Math::Pixel movement)
    {
        center_.first += movement.first;
        center_.second += movement.second;
        for(auto& element : elements_)
        {
            Math::Pixel pixel = Math::IndexToPixel(element.first, elementRadius_, center_);
            element.second.x_ = pixel.first;
            element.second.y_ = pixel.second;
        }
    }

    Math::Pixel Logo::Symbol::GetPosition() const
    {
        return center_;
    }

    int16_t Logo::Symbol::CalculateBestSpacing(const Symbol& right) const
    {
        // TODO: take in to account diagonal spacings as well
        int16_t spacing = 10;
        int16_t closest = 10;
        for(const auto& hexA : elements_)
        {
            int16_t qA = std::get<0>(hexA.first);
            int16_t rA = std::get<1>(hexA.first);
            for(const auto& hexB : right.elements_)
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

    void Logo::Symbol::Draw() const
    {
        for(auto& element : elements_)
            element.second.Draw();
    }

    void Logo::Symbol::AddHexagon(int16_t q, int16_t r)
    {
        Math::Index index{q, r, -q - r};
        Math::Pixel pixel = Math::IndexToPixel(index, elementRadius_, center_);
        elements_.emplace(index, Hexagon{pixel.first, pixel.second, elementRadius_, 1});
    }

    void Logo::Symbol::GenerateH()
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

    void Logo::Symbol::GenerateE()
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

    void Logo::Symbol::GenerateX()
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

    void Logo::Symbol::GenerateV()
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

    void Logo::Symbol::GenerateO()
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

    void Logo::Symbol::GenerateI()
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

    void Logo::Symbol::GenerateD()
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

    void Logo::Symbol::GenerateSeparator()
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
