
#include "hex.hpp"

namespace hex
{
    size_t Score::score_ = 0;
    size_t Score::moves_ = 0;
    uint16_t Score::movesLeft_ = 0;
    Text Score::text_;

    void Score::Start()
    {
        text_ = Text(14, 10, 10, 3);

        score_ = 0;
        moves_ = 0;
        movesLeft_ = 10;

        text_.Set("Score: " + std::to_string(score_), 0);
        text_.Set("Moves: " + std::to_string(moves_), 1);
        text_.Set("Moves left: " + std::to_string(movesLeft_), 2);
    }

    void Score::AddScore(uint16_t points)
    {
        score_ += points;
        text_.Set("Score: " + std::to_string(score_), 0);
    }
    void Score::TakeScore(uint16_t points)
    {
        if(score_ > points)
            score_ -= points;
        else
            score_ = 0;

        text_.Set("Score: " + std::to_string(score_), 0);
    }

    void Score::AddMoves(uint16_t points)
    {
        movesLeft_ += points;
        text_.Set("Moves left: " + std::to_string(movesLeft_), 2);
    }
    void Score::TakeMoves(uint16_t points)
    {
        if(movesLeft_ > points)
            movesLeft_ -= points;
        else
            movesLeft_ = 0;

        text_.Set("Moves left: " + std::to_string(movesLeft_), 2);
    }

    void Score::RegisterMove()
    {
        moves_++;
        text_.Set("Moves: " + std::to_string(moves_), 1);
    }

    void Score::Draw()
    {
        printf("SCORE\n");

        text_.Draw();
    }

} // namespace hex
