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

} // namespace hex
