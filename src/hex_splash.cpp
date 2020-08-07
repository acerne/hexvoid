#include <algorithm>
#include <random>
#include <stdexcept>

#include "hexagon.hpp"

namespace hex
{

    Splash::Splash(const std::string& title, Tiling::Pixel center, double hexRadius)
    {
        elementRadius_ = hexRadius;

        logotype_.clear();
        for(const char& character : title)
            logotype_.push_back(Symbol{character, center, elementRadius_});

        std::vector<int16_t> spacing(logotype_.size());
        spacing.front() = 0;
        for(int i = 1; i < logotype_.size(); i++)
            spacing.at(i) = spacing.at(i - 1) + logotype_.at(i - 1).CalculateBestSpacing(logotype_.at(i));

        int16_t totalSpacing = spacing.back();
        double hexWidth = 2 * Tiling::RadiusToApothem(elementRadius_);
        double xOffset = center.first - totalSpacing * hexWidth / 2;
        double y = center.second;

        int16_t distanceFromFirstSymbol = 0;
        for(int i = 0; i < logotype_.size(); i++)
        {
            double x = xOffset + spacing.at(i) * hexWidth;
            logotype_.at(i).SetPosition({x, y});
        }
    }

    void Splash::Draw() const
    {
        for(auto& symbol : logotype_)
            symbol.Draw();
    }

} // namespace hex
