#pragma once

#include <cstddef>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace hexvoid
{
    class Text
    {
    public:
        Text(int fontSize);
        ~Text();

        void Position(int16_t x, int16_t y);
        void Apply(const std::string& text);

        void Draw(SDL_Renderer*& gRenderer) const;

    private:
        TTF_Font* font_;
        std::string text_;
        int16_t x_;
        int16_t y_;
    };

    class Score
    {
    public:
        Score();

        void AddScore(uint16_t points);
        void AddMoves(uint8_t moves);
        void Move();
        void Undo();
        void GameOver();

        void Draw(SDL_Renderer*& gRenderer) const;

    private:
        size_t score_;
        uint8_t movesLeft_;
        uint8_t undosLeft_;
        Text scoreText_ = Text(26);
        Text movesText_ = Text(20);
        Text undosText_ = Text(20);
    };

} // namespace hexvoid