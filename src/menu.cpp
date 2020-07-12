#include "menu.hpp"

#include <limits>
#include <stdexcept>

namespace hexvoid
{
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
        printf("\tScore: %zu\n", score_);
    }

    // void Score::Draw(SDL_Renderer*& gRenderer) const
    // {
    //     scoreText_.Draw(gRenderer);
    //     movesText_.Draw(gRenderer);
    //     undosText_.Draw(gRenderer);
    // }

} // namespace hexvoid
