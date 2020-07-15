#include "hex.hpp"

namespace hex
{
    // source: https://www.shutterstock.com/blog/neon-color-palettes
    const std::map<std::string, Palette::Theme> Palette::themes_ = {
        {"Geometric Glow", {0x000000, 0xFFFFFF, 0x08F7FE, 0x09FBBB, 0xFE53BB, 0xF5D300}},
        {"Neon Lights", {0x000000, 0xFFFFFF, 0xFFACFC, 0xF148FB, 0x7122FA, 0x560A86}},
        {"Psychedelic Pattern", {0x000000, 0xFFFFFF, 0xdd75D5FD, 0xB76CFD, 0xFF2281, 0x011FFD}},
        {"Luminous Lines", {0x000000, 0xFFFFFF, 0x01FFC3, 0x01FFFF, 0xFFB3FD, 0x9D72FF}},
        {"Vivid Ferns", {0x000000, 0xFFFFFF, 0xFDC7D7, 0xFF9DE6, 0xA5D8F3, 0xE8E500}},
        {"Abstract Wall", {0x000000, 0xFFFFFF, 0x00FECA, 0xFDF200, 0xFF85EA, 0x7B61F8}},
        {"Colorful Cottage", {0x000000, 0xFFFFFF, 0xFFD300, 0xDE38C8, 0x652EC7, 0x33135C}},
        {"Cosmic Dust", {0x202020, 0x808080, 0x3B27BA, 0xE847AE, 0x13CA91, 0xFF9472}},
        {"Pops Of Pink", {0x000000, 0xFFFFFF, 0xFFDEF3, 0xFF61BE, 0x3B55CE, 0x35212A}},
        {"Flourescent Fish", {0x000000, 0xFFFFFF, 0xFEC763, 0xEA55B1, 0xA992FA, 0x00207F}}};

    std::string Palette::selectedTheme_ = "Geometric Glow";

    void Palette::ChangeTheme(const std::string& themeName)
    {
        if(themes_.find(themeName) != themes_.end()) selectedTheme_ = themeName;
    }

    Palette::Color Palette::GetColor(Element id)
    {
        switch(id)
        {
            case Element::Background:
                return themes_.at(selectedTheme_).background;
            case Element::Foreground:
                return themes_.at(selectedTheme_).foreground;
            case Element::A:
                return themes_.at(selectedTheme_).A;
            case Element::B:
                return themes_.at(selectedTheme_).B;
            case Element::C:
                return themes_.at(selectedTheme_).C;
            case Element::D:
                return themes_.at(selectedTheme_).D;
            default:
                throw std::runtime_error("Invalid color id");
        }
    }

    std::string Palette::GetThemeName()
    {
        return selectedTheme_;
    }

    std::vector<std::string> Palette::GetThemeNames()
    {
        std::vector<std::string> names;
        names.reserve(themes_.size());

        for(const auto& theme : themes_)
            names.push_back(theme.first);

        return std::move(names);
    }

} // namespace hex
