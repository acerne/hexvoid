#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Menu::Menu(uint16_t spacing)
    {
        items_.clear();
        spacing_ = spacing;
    }

    void Menu::AddItem(const std::string& title, uint8_t index)
    {
        int x = Engine::windowWidth_ / 2;
        int y = spacing_ * (index + 1);
        SDL_Rect bounds = FC_GetBounds(font_, x, y, FC_AlignEnum::FC_ALIGN_CENTER, {1, 1}, "%s", title.c_str());
        items_[title] = MenuItem{MenuItemType::Item, title, index, bounds, {}, 0};
    }
    void Menu::AddOption(const std::string& title, uint8_t index, const std::vector<std::string>& valueNames)
    {
        int x = Engine::windowWidth_ / 5;
        int y = spacing_ * (index + 1);
        SDL_Rect bounds = FC_GetBounds(font_, x, y, FC_AlignEnum::FC_ALIGN_CENTER, {1, 1}, "%s", title.c_str());
        items_[title] = MenuItem{MenuItemType::Option, title, index, bounds, std::move(valueNames), 0};
    }

    bool Menu::IsMouseInside(int16_t x, int16_t y, const SDL_Rect& box) const
    {
        return (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h);
    }

    int8_t Menu::MouseOverItemIndex(int16_t cursorX, int16_t cursorY) const
    {
        for(const auto& item : items_)
        {
            if(IsMouseInside(cursorX, cursorY, item.second.box)) return item.second.index;
        }
        return -1;
    }

    std::string Menu::MouseOverItemName(int16_t cursorX, int16_t cursorY) const
    {
        for(const auto& item : items_)
        {
            if(IsMouseInside(cursorX, cursorY, item.second.box)) return item.second.title;
        }
        return "";
    }

    void Menu::NextOption(const std::string& itemName)
    {
        items_.at(itemName).selected++;
        if(items_.at(itemName).selected >= items_.at(itemName).options.size()) items_.at(itemName).selected = 0;
    }

    int8_t Menu::Click(int16_t cursorX, int16_t cursorY)
    {
        std::string selected = MouseOverItemName(cursorX, cursorY);
        if(selected.length() > 0)
        {
            switch(items_.at(selected).type)
            {
                case MenuItemType::Item: {
                    return items_[selected].index;
                    break;
                }
                case MenuItemType::Option: {
                    NextOption(selected);
                    return items_[selected].index;
                    break;
                }
                default:
                    break;
            }
        }
        return -1;
    }

    std::string Menu::GetSelection(const std::string& item) const
    {
        return items_.at(item).options.at(items_.at(item).selected);
    }

    void Menu::Draw(int16_t cursorX, int16_t cursorY) const
    {

        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);
        Palette::Color c = Palette::GetColor(Palette::Element::A);

        uint8_t hover = MouseOverItemIndex(cursorX, cursorY);
        for(const auto& item : items_)
        {
            SDL_Color color = {f.r, f.g, f.b, 255};
            if(item.second.index == hover) color = {c.r, c.g, c.b, 255};

            switch(item.second.type)
            {
                case MenuItemType::Item: {
                    FC_SetDefaultColor(font_, color);
                    FC_DrawScale(font_, gRenderer_, item.second.box.x, item.second.box.y, {1, 1}, "%s",
                                 item.second.title.c_str());

                    break;
                }
                case MenuItemType::Option: {
                    FC_SetDefaultColor(font_, {f.r, f.g, f.b, 255});
                    FC_DrawScale(font_, gRenderer_, item.second.box.x, item.second.box.y, {1, 1}, "%s",
                                 item.second.title.c_str());

                    int x = item.second.box.x + item.second.box.w + 20;
                    int y = item.second.box.y;
                    FC_SetDefaultColor(font_, color);
                    FC_DrawScale(font_, gRenderer_, x, y, {1, 1}, "%s",
                                 item.second.options.at(item.second.selected).c_str());

                    break;
                }
                default:
                    break;
            }
        }
    }

} // namespace hex
