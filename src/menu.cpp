#include "menu.hpp"

#include <limits>
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

    void Text::Draw(SDL_Renderer*& gRenderer) const
    {
        const char* text_c = text_.c_str();
        SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_c, {255, 255, 255});
        int w, h;
        TTF_SizeText(font_, text_c, &w, &h);
        SDL_Rect textLocation = {x_, y_, w, h};
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, Message, NULL, &textLocation);
    }

    Score::Score()
    {
        score_ = 0;
        movesLeft_ = 10;
        undosLeft_ = 1;
        scoreText_.Position(5, 100);
        movesText_.Position(5, 130);
        undosText_.Position(5, 150);
        scoreText_.Apply("SCORE: " + std::to_string(score_));
        movesText_.Apply("Moves left: " + std::to_string(movesLeft_));
        undosText_.Apply("Undos left: " + std::to_string(undosLeft_));
    }

    void Score::AddScore(uint16_t points)
    {
        score_ += points;
        scoreText_.Apply("SCORE: " + std::to_string(score_));
    }

    void Score::AddMoves(uint8_t moves)
    {
        uint8_t maxValue = std::numeric_limits<uint8_t>::max();

        if(movesLeft_ < maxValue)
        {
            if(movesLeft_ > maxValue - moves)
                movesLeft_ = maxValue;
            else
                movesLeft_ += moves;
        }
        movesText_.Apply("Moves left " + std::to_string(movesLeft_));
    }

    void Score::Move()
    {
        if(movesLeft_ > 0)
            movesLeft_--;
        else
            GameOver();

        movesText_.Apply("Moves left: " + std::to_string(movesLeft_));
    }

    void Score::GameOver()
    {
        printf("G A M E   O V E R !\n");
        printf("\tScore: %i\n", score_);
    }

    void Score::Draw(SDL_Renderer*& gRenderer) const
    {
        scoreText_.Draw(gRenderer);
        movesText_.Draw(gRenderer);
        undosText_.Draw(gRenderer);
    }

} // namespace hexvoid
