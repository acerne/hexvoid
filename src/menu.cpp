#include "menu.hpp"

#include <stdexcept>

namespace hexvoid
{
    Text::Text(int fontSize)
    {
        font_ = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", fontSize); // select and move font
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        text_ = "";
        x_ = 0;
        y_ = 0;
    }

    Text::~Text()
    {
        TTF_CloseFont(font_);
    }

    void Text::Position(int16_t x, int16_t y)
    {
        x_ = x;
        y_ = y;
    }

    void Text::Apply(const std::string& text)
    {
        text_ = text;
    }

    void Text::Draw(SDL_Renderer*& gRenderer)
    {
        const char* text_c = text_.c_str();
        SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_c, {255, 255, 255});
        int w, h;
        TTF_SizeText(font_, text_c, &w, &h);
        SDL_Rect textLocation = {x_, y_, w, h};
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, Message, NULL, &textLocation);
    }

} // namespace hexvoid
