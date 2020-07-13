#pragma once

#include "hex.hpp"

namespace hexvoid
{

    class Score
    {
    public:
        Score();

        void AddScore(uint16_t points);
        void AddMoves(uint8_t moves);
        void Move();
        void Undo();
        void GameOver();

        void Draw() const;

    private:
        size_t score_;
        uint8_t movesLeft_;
        uint8_t undosLeft_;
        hex::Text scoreText_ = hex::Text(26);
        hex::Text movesText_ = hex::Text(20);
        hex::Text undosText_ = hex::Text(20);
    };

} // namespace hexvoid