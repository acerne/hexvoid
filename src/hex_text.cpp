#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Text::Text(int fontSize, int16_t x, int16_t y, uint8_t lines)
    {
        font_ = TTF_OpenFont(Engine::fontPath_, fontSize); // select and move font
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        text_ = std::vector<std::string>(lines);
        for(auto& line : text_)
            line = "";

        lines_ = lines;
        x_ = x;
        y_ = y;
        initialized_ = true;
    }

    Text::~Text()
    {
        if(initialized_) TTF_CloseFont(font_);
    }

    void Text::Place(int16_t x, int16_t y)
    {
        x_ = x;
        y_ = y;
    }

    void Text::Set(const std::string& text, uint8_t line)
    {
        if(line < lines_)
            text_.at(line) = std::move(text);
        else
            throw std::out_of_range("Text line out of range: " + std::to_string(line) + " / " + std::to_string(lines_));
    }

    void Text::Draw() const
    {
        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);

        for(uint8_t i = 0; i < lines_; i++)
        {
            printf(" %i: (%i,%i) %s\n", i, x_, y_, text_.at(i).c_str());

            const char* text_c = text_.at(i).c_str();
            SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_c, {f.r, f.g, f.b});
            int w, h;
            Err(TTF_SizeText(font_, text_c, &w, &h));
            SDL_Rect textLocation = {x_, y_ + i * h, w, h};
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Engine::gRenderer_, textSurface);
            Err(SDL_RenderCopy(Engine::gRenderer_, textTexture, NULL, &textLocation));
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }

} // namespace hex
