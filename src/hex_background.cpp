
#include "hexagon.hpp"

namespace hex
{
    FadeOut::FadeOut(int16_t size, double hexRadius, uint8_t maxAlpha)
    {
        maxAlpha_ = maxAlpha;
        grid_ = Grid(size, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(100))
                randomColor.a = Randomizer::Random(0, maxAlpha_);
            else
                randomColor.a = 0;
            colors_.emplace(index, randomColor);
        }
    }

    void FadeOut::UpdatePhysics()
    {
        for(auto& [index, color] : colors_)
        {
            if(color.a == 0)
            {
                if(Randomizer::Chance(1000))
                {
                    color = Palette::RandomColor();
                    color.a = Randomizer::Random(0, maxAlpha_);
                }
            }
            else
                color.a--;
        }
    }

    void FadeOut::Draw()
    {
        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            if(colors_.at(index).a > 0) hexagon.Draw(colors_.at(index));
        }
    }

    FadeInFadeOut::FadeInFadeOut(int16_t size, double hexRadius)
    {
        grid_ = Grid(size, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(5000))
                randomColor.a = Randomizer::Random(0, 255);
            else
                randomColor.a = 127;

            colors_.emplace(index, randomColor);
        }
    }

    void FadeInFadeOut::UpdatePhysics()
    {
        for(auto& [index, color] : colors_)
        {
            if(color.a == 127)
            {
                if(Randomizer::Chance(5000))
                {
                    color = Palette::RandomColor();
                    color.a = 128;
                }
            }
            else
                color.a++;
        }
    }

    void FadeInFadeOut::Draw()
    {
        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color temp = colors_.at(index);

            if(temp.a > 127)
            {
                temp.a = temp.a - 127;
                hexagon.Draw(temp);
            }
            else if(temp.a < 127)
            {
                temp.a = 127 - temp.a;
                hexagon.Draw(temp);
            }
        }
    }

} // namespace hex
