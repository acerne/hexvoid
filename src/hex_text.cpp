#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    Text::Text(int fontSize, uint8_t lines)
    {
        if(debug_) printf("Creating Text class...\n");

        font_ = TTF_OpenFont(Engine::fontPath_, fontSize); // select and move font
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        text_ = std::vector<std::string>(lines);
        for(auto& line : text_)
            line = "";

        lines_ = lines;
        x_ = 0;
        y_ = 0;
    }

    Text::~Text()
    {
        TTF_CloseFont(font_);
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
        for(uint8_t i = 0; i < lines_; i++)
        {
            const char* text_c = text_.at(i).c_str();
            SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_c, {255, 255, 255});
            int w, h;
            TTF_SizeText(font_, text_c, &w, &h);
            SDL_Rect textLocation = {x_, y_ + i * h, w, h};
            SDL_Texture* Message = SDL_CreateTextureFromSurface(Engine::gRenderer_, textSurface);
            SDL_RenderCopy(Engine::gRenderer_, Message, NULL, &textLocation);
            SDL_DestroyTexture(Message);
            SDL_FreeSurface(textSurface);
        }
    }

} // namespace hex
