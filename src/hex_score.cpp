
#include "hex.hpp"

namespace hex
{
    size_t Score::score_ = 0;
    size_t Score::moves_ = 0;
    uint16_t Score::movesLeft_ = 0;

    void Score::Start()
    {
        score_ = 0;
        moves_ = 0;
        movesLeft_ = 10;
    }

    void Score::AddScore(uint16_t points)
    {
        score_ += points;
    }
    void Score::TakeScore(uint16_t points)
    {
        if(score_ > points)
            score_ -= points;
        else
            score_ = 0;
    }

    void Score::AddMoves(uint16_t points)
    {
        movesLeft_ += points;
    }
    void Score::TakeMoves(uint16_t points)
    {
        if(movesLeft_ > points)
            movesLeft_ -= points;
        else
            movesLeft_ = 0;
    }

    void Score::RegisterMove()
    {
        moves_++;
    }

    void Score::Draw()
    {
        const char* formattedScore = "Score: %i\nMoves: %i\nMoves left: %i";

        FC_SetDefaultColor(font_, {255, 255, 255, 255});
        FC_DrawScale(font_, gRenderer_, 5, 500, {0.5, 0.5}, formattedScore, score_, moves_, movesLeft_);
    }

} // namespace hex
