#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Menu::Menu(uint16_t spacing, int fontSize)
    {
        font_ = TTF_OpenFont(Engine::fontPath_, fontSize); // select and move font
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        items_.clear();
        spacing_ = spacing;
    }

    Menu::~Menu()
    {
        TTF_CloseFont(font_);
    }

    void Menu::AddItem(const std::string& title, uint8_t index)
    {
        const char* text_c = title.c_str();
        int w, h;
        TTF_SizeText(font_, text_c, &w, &h);
        int x = Engine::windowWidth_ / 2 - w / 2;
        int y = spacing_ * (index + 1);

        items_[title] = MenuItem{MenuItemType::Item, title, index, SDL_Rect{x, y, w, h}, {}, 0};
    }
    void Menu::AddOption(const std::string& title, uint8_t index, const std::vector<std::string>& valueNames)
    {
        const char* text_c = title.c_str();
        int w, h;
        TTF_SizeText(font_, text_c, &w, &h);
        int x = Engine::windowWidth_ / 5;
        int y = spacing_ * (index + 1);

        items_[title] = MenuItem{MenuItemType::Option, title, index, SDL_Rect{x, y, w, h}, std::move(valueNames), 0};
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
            SDL_Color color = {f.r, f.g, f.b};
            if(item.second.index == hover) color = {c.r, c.g, c.b};
            switch(item.second.type)
            {
                case MenuItemType::Item: {
                    const char* itemText_c = item.second.title.c_str();
                    SDL_Surface* itemSurface = TTF_RenderText_Solid(font_, itemText_c, color);
                    SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(Engine::gRenderer_, itemSurface);
                    SDL_RenderCopy(Engine::gRenderer_, itemTexture, NULL, &item.second.box);
                    SDL_DestroyTexture(itemTexture);
                    SDL_FreeSurface(itemSurface);
                    break;
                }
                case MenuItemType::Option: {
                    const char* itemText_c = item.second.title.c_str();
                    SDL_Surface* itemSurface = TTF_RenderText_Solid(font_, itemText_c, {f.r, f.g, f.b});

                    const char* optionText_c = item.second.options.at(item.second.selected).c_str();
                    int w, h;
                    TTF_SizeText(font_, optionText_c, &w, &h);
                    int x = item.second.box.x + item.second.box.w + 20;
                    int y = item.second.box.y;
                    SDL_Rect optionBox = {x, y, w, h};
                    SDL_Surface* optionSurface = TTF_RenderText_Solid(font_, optionText_c, color);

                    SDL_Texture* ItemTexture = SDL_CreateTextureFromSurface(Engine::gRenderer_, itemSurface);
                    SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(Engine::gRenderer_, optionSurface);
                    SDL_RenderCopy(Engine::gRenderer_, ItemTexture, NULL, &item.second.box);
                    SDL_RenderCopy(Engine::gRenderer_, optionTexture, NULL, &optionBox);
                    SDL_DestroyTexture(ItemTexture);
                    SDL_DestroyTexture(optionTexture);
                    SDL_FreeSurface(itemSurface);
                    SDL_FreeSurface(optionSurface);
                    break;
                }
                default:
                    break;
            }
        }
    }

} // namespace hex
