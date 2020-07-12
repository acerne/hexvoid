#include "hex.hpp"

namespace hex
{
    Palette::Palette()
    {
        if(debug_) printf("Creating Palette class...\n");

        selectedTheme_ = 0;

        // source: https://www.shutterstock.com/blog/neon-color-palettes
        themeList_ = {{"Geometric Glow", Color(0x000000), Color(0xFFFFFF), Color(0x08F7FE), Color(0x09FBBB),
                       Color(0xFE53BB), Color(0xF5D300)},
                      {"Neon Lights", Color(0x000000), Color(0xFFFFFF), Color(0xFFACFC), Color(0xF148FB),
                       Color(0x7122FA), Color(0x560A86)},
                      {"Psychedelic Pattern", Color(0x000000), Color(0xFFFFFF), Color(0x75D5FD), Color(0xB76CFD),
                       Color(0xFF2281), Color(0x011FFD)},
                      {"Luminous Lines", Color(0x000000), Color(0xFFFFFF), Color(0x01FFC3), Color(0x01FFFF),
                       Color(0xFFB3FD), Color(0x9D72FF)},
                      {"Vivid Ferns", Color(0x000000), Color(0xFFFFFF), Color(0xFDC7D7), Color(0xFF9DE6),
                       Color(0xA5D8F3), Color(0xE8E500)},
                      {"Abstract Wall", Color(0x000000), Color(0xFFFFFF), Color(0x00FECA), Color(0xFDF200),
                       Color(0xFF85EA), Color(0x7B61F8)},
                      {"Colorful Cottage", Color(0x000000), Color(0xFFFFFF), Color(0xFFD300), Color(0xDE38C8),
                       Color(0x652EC7), Color(0x33135C)},
                      {"Cosmic Dust", Color(0x202020), Color(0x808080), Color(0x3B27BA), Color(0xE847AE),
                       Color(0x13CA91), Color(0xFF9472)},
                      {"Pops Of Pink", Color(0x000000), Color(0xFFFFFF), Color(0xFFDEF3), Color(0xFF61BE),
                       Color(0x3B55CE), Color(0x35212A)},
                      {"Flourescent Fish", Color(0x000000), Color(0xFFFFFF), Color(0xFEC763), Color(0xEA55B1),
                       Color(0xA992FA), Color(0x00207F)}};

        text_.Set(GetThemeName());
        text_.Place(5, 570);
    }

    Palette::Color Palette::GetThemeColor(int index) const
    {
        switch(index)
        {
            case 0:
                return themeList_.at(selectedTheme_).background;
            case 1:
                return themeList_.at(selectedTheme_).foreground;
            case 2:
                return themeList_.at(selectedTheme_).A;
            case 3:
                return themeList_.at(selectedTheme_).B;
            case 4:
                return themeList_.at(selectedTheme_).C;
            case 5:
                return themeList_.at(selectedTheme_).D;
            default:
                throw std::runtime_error("Invalid color index");
        }
    }

    std::string Palette::GetThemeName() const
    {
        return themeList_.at(selectedTheme_).name;
    }

    void Palette::NextTheme()
    {
        if(selectedTheme_ < themeList_.size() - 1)
        {
            selectedTheme_++;
            text_.Set(GetThemeName());
        }
    }

    void Palette::PreviousTheme()
    {
        if(selectedTheme_ > 0)
        {
            selectedTheme_--;
            text_.Set(GetThemeName());
        }
    }

    void Palette::DrawInfo()
    {
        text_.Draw();
    };

} // namespace hex
