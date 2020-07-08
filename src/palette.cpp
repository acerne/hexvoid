#include "palette.hpp"

namespace hexvoid
{
    Palette::Palette()
    {
        selectedTheme_ = 0;

        // source: https://www.shutterstock.com/blog/neon-color-palettes
        themeList_ = {
            {"GeometricGlow", Color(0x000000), Color(0x08F7FE), Color(0x09FBBB), Color(0xFE53BB), Color(0xF5D300)},
            {"NeonLights", Color(0x000000), Color(0xFFACFC), Color(0xF148FB), Color(0x7122FA), Color(0x560A86)},
            {"PsychedelicPattern", Color(0x000000), Color(0x75D5FD), Color(0xB76CFD), Color(0xFF2281), Color(0x011FFD)},
            {"LuminousLines", Color(0x000000), Color(0x01FFC3), Color(0x01FFFF), Color(0xFFB3FD), Color(0x9D72FF)},
            {"VividFerns", Color(0x000000), Color(0xFDC7D7), Color(0xFF9DE6), Color(0xA5D8F3), Color(0xE8E500)},
            {"AbstractWall", Color(0x000000), Color(0x00FECA), Color(0xFDF200), Color(0xFF85EA), Color(0x7B61F8)},
            {"ColorfulCottage", Color(0x000000), Color(0xFFD300), Color(0xDE38C8), Color(0x652EC7), Color(0x33135C)},
            {"CosmicDust", Color(0x000000), Color(0x3B27BA), Color(0xE847AE), Color(0x13CA91), Color(0xFF9472)},
            {"PopsOfPink", Color(0x000000), Color(0xFFDEF3), Color(0xFF61BE), Color(0x3B55CE), Color(0x35212A)},
            {"FlourescentFish", Color(0x000000), Color(0xFEC763), Color(0xEA55B1), Color(0xA992FA), Color(0x00207F)}};
    }

    Palette::Color Palette::GetThemeColor(int index)
    {
        switch(index)
        {
            case 0:
                return themeList_.at(selectedTheme_).background;
            case 1:
                return themeList_.at(selectedTheme_).A;
            case 2:
                return themeList_.at(selectedTheme_).B;
            case 3:
                return themeList_.at(selectedTheme_).C;
            case 4:
                return themeList_.at(selectedTheme_).D;
            default:
                throw std::runtime_error("Invalid color index");
        }
    }

    std::string Palette::GetThemeName()
    {
        return themeList_.at(selectedTheme_).name;
    }

    void Palette::NextTheme()
    {
        if(selectedTheme_ < themeList_.size() - 1) selectedTheme_++;
    }

    void Palette::PreviousTheme()
    {
        if(selectedTheme_ > 0) selectedTheme_--;
    }

} // namespace hexvoid
