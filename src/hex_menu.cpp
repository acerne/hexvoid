#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Menu::Menu(uint8_t items, uint16_t spacing, int fontSize)
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

        items_[title] = MenuItem{title, index, SDL_Rect{x, y, w, h}};
    }

    bool Menu::IsMouseInside(int16_t x, int16_t y, const SDL_Rect& box) const
    {
        return (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h);
    }

    uint8_t Menu::MouseOver(int16_t cursorX, int16_t cursorY) const
    {
        for(const auto& item : items_)
        {
            if(IsMouseInside(cursorX, cursorY, item.second.box)) return item.second.index;
        }
        return 255;
    }

    void Menu::Draw(int16_t cursorX, int16_t cursorY) const
    {
        uint8_t hover = MouseOver(cursorX, cursorY);
        for(const auto& item : items_)
        {
            SDL_Color color = {255, 255, 255};
            if(item.second.index == hover) color = {255, 127, 0};
            const char* text_c = item.second.title.c_str();
            SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_c, color);
            SDL_Texture* Message = SDL_CreateTextureFromSurface(Engine::gRenderer_, textSurface);
            SDL_RenderCopy(Engine::gRenderer_, Message, NULL, &item.second.box);
            SDL_DestroyTexture(Message);
            SDL_FreeSurface(textSurface);
        }
    }

} // namespace hex
