
#include "hexbackground.hpp"

namespace hex
{
    FadeOut::FadeOut(int16_t sizeX, int16_t sizeY, double hexRadius, uint8_t maxAlpha)
    {
        maxAlpha_ = maxAlpha;
        grid_ = RectangularGrid(sizeX, sizeY, hexRadius);
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

    FadeIn::FadeIn(int16_t sizeX, int16_t sizeY, double hexRadius, uint8_t maxAlpha)
    {
        maxAlpha_ = maxAlpha;
        grid_ = RectangularGrid(sizeX, sizeY, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(100))
                randomColor.a = Randomizer::Random(0, maxAlpha_);
            else
                randomColor.a = maxAlpha_;
            colors_.emplace(index, randomColor);
        }
    }

    void FadeIn::UpdatePhysics()
    {
        for(auto& [index, color] : colors_)
        {
            if(color.a >= maxAlpha_)
            {
                if(Randomizer::Chance(2000))
                {
                    color = Palette::RandomColor();
                    color.a = 0;
                }
            }
            else
                color.a++;
        }
    }

    void FadeIn::Draw()
    {
        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            if(colors_.at(index).a > 0 && colors_.at(index).a < maxAlpha_) hexagon.Draw(colors_.at(index));
        }
    }

    FadeInFadeOut::FadeInFadeOut(int16_t sizeX, int16_t sizeY, double hexRadius)
    {
        grid_ = RectangularGrid(sizeX, sizeY, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(100))
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

    Twinkle::Twinkle(int16_t sizeX, int16_t sizeY, double hexRadius)
    {
        grid_ = RectangularGrid(sizeX, sizeY, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(100))
                randomColor.a = Randomizer::Random(0, 32) * flashDivider - 1;
            else
                randomColor.a = 127;

            colors_.emplace(index, randomColor);
        }
    }

    void Twinkle::UpdatePhysics()
    {
        for(auto& [index, color] : colors_)
        {
            if(color.a == 127)
            {
                if(Randomizer::Chance(6000))
                {
                    color = Palette::RandomColor();
                    color.a = 127 + flashDivider;
                }
            }
            else
                color.a += flashDivider;
        }
    }

    void Twinkle::Draw()
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

    Snakes::Snakes(int16_t sizeX, int16_t sizeY, double hexRadius)
    {
        grid_ = RectangularGrid(sizeX, sizeY, hexRadius);
        colors_.clear();

        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color randomColor = Palette::RandomColor();
            if(Randomizer::Chance(100))
                randomColor.a = Randomizer::Random(0, 32);
            else
                randomColor.a = 0;

            colors_.emplace(index, randomColor);
        }
    }

    void Snakes::UpdatePhysics()
    {
        for(auto& [index, color] : colors_)
        {
            if(color.a == 32)
            {
                uint8_t direction = Randomizer::Random(0, 5);
                switch(direction)
                {
                    case 0: {
                        Tiling::Index neighbour = index + Tiling::Index{0, 1, -1};
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    case 1: {
                        Tiling::Index neighbour = index + Tiling::Index{0, -1, 1};
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    case 2: {
                        Tiling::Index neighbour = index + Tiling::Index{1, 0, -1};
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    case 3: {
                        Tiling::Index neighbour = index + Tiling::Index{-1, 0, 1};
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    case 4: {
                        Tiling::Index neighbour = index + Tiling::Index(1, -1, 0);
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    case 5: {
                        Tiling::Index neighbour = index + Tiling::Index{-1, 1, 0};
                        if(colors_.find(neighbour) != colors_.end())
                        {
                            if(colors_.at(neighbour).a == 0)
                            {
                                colors_.at(neighbour) = color;
                                colors_.at(neighbour).a = 1;
                                break;
                            }
                        }
                    }
                    default:
                        break;
                }
                color.a++;
            }
            else if(color.a == 0)
            {
                if(Randomizer::Chance(10000))
                {
                    color = Palette::RandomColor();
                    color.a = 1;
                }
            }
            else
                color.a++;
        }
    }

    void Snakes::Draw()
    {
        for(const auto& [index, hexagon] : grid_.GetTiles())
        {
            Palette::Color temp = colors_.at(index);

            if(temp.a > 127)
            {
                if(temp.a < 192)
                {
                    temp.a = 127;
                    hexagon.Draw(temp);
                }
                else
                {
                    temp.a = 127 - (temp.a - 192);
                    hexagon.Draw(temp);
                }
            }
            else if(temp.a > 0)
            {
                hexagon.Draw(temp);
            }
        }
    }

} // namespace hex
