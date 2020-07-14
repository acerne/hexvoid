#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Menu::Menu(uint16_t spacing, int fontSize)
    {
        if(debug_) printf("Creating Menu class...\n");

        font_ = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", fontSize); // select and move font
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
        items_[itemName].selected++;
        if(items_[itemName].selected >= items_[itemName].options.size()) items_[itemName].selected = 0;
    }

    int8_t Menu::Click(int16_t cursorX, int16_t cursorY)
    {
        std::string selected = MouseOverItemName(cursorX, cursorY);
        if(selected.length() > 0)
        {
            switch(items_[selected].type)
            {
                case MenuItemType::Item: {
                    return items_[selected].index;
                    break;
                }
                case MenuItemType::Option: {
                    NextOption(selected);
                    break;
                }
                default:
                    break;
            }
        }
        return -1;
    }

    void Menu::Draw(int16_t cursorX, int16_t cursorY) const
    {
        uint8_t hover = MouseOverItemIndex(cursorX, cursorY);
        for(const auto& item : items_)
        {
            SDL_Color color = {255, 255, 255};
            if(item.second.index == hover) color = {255, 127, 0};
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
                    SDL_Surface* itemSurface = TTF_RenderText_Solid(font_, itemText_c, {255, 255, 255});

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
