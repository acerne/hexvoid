#include "hexvoid.hpp"

using namespace hex;

namespace hexvoid
{
    Grid hexvoid::Game::gameGrid_;

    void Game::Initialize()
    {
        // Grid grid(3);
        gameGrid_ = Grid(9);
    }

    void Game::Update()
    {
        switch(Engine::GetGameState())
        {
            case Engine::GameState::GAME: {
                SDL_Event clickEvent = Input::GetClick();
                switch(clickEvent.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        RotateClockwise(clickEvent.motion.x, clickEvent.motion.y);
                        break;
                    case SDL_BUTTON_RIGHT:
                        RotateCounterClockwise(clickEvent.motion.x, clickEvent.motion.y);
                        break;
                    default:
                        break;
                }
                // if(Score::IsGameOver())
                // {
                //     startTime = std::chrono::system_clock::now();
                //     state = GameState::GAME_OVER;
                // }
                break;
            }
            default:
                break;
        }
    }

    void Game::Draw()
    {
        switch(Engine::GetGameState())
        {
            case Engine::GameState::GAME:
                gameGrid_.Draw(Input::cursorX, Input::cursorY);
                Score::Draw();
                break;
            default:
                break;
        }
    }

    void Game::RotateClockwise(int16_t cursorX, int16_t cursorY)
    {
        auto [valid, index] = gameGrid_.GetHoveringIndex({cursorX, cursorY});
        if(valid)
        {
            bool inner = Tiling::IndexDistance({0, 0, 0}, index) < (gameGrid_.GetGridSize() - 1) / 2 - 1;
            if(inner)
            {
                if(CheckSolution(index))
                {
                    Score::AddScore(100);
                    Score::AddMoves(10);
                    ShuffleSolution(index);
                }
                else
                {
                    Score::TakeMoves(1);
                    gameGrid_.Rotate(index, 1, 1);
                }
                Score::RegisterMove();
            }
        }
    }

    void Game::RotateCounterClockwise(int16_t cursorX, int16_t cursorY)
    {
        auto [valid, index] = gameGrid_.GetHoveringIndex({cursorX, cursorY});
        if(valid)
        {
            bool inner = Tiling::IndexDistance({0, 0, 0}, index) < (gameGrid_.GetGridSize() - 1) / 2 - 1;
            if(inner)
            {
                if(CheckSolution(index))
                {
                    Score::AddScore(100);
                    Score::AddMoves(10);
                    ShuffleSolution(index);
                }
                else
                {
                    Score::TakeMoves(1);
                    gameGrid_.Rotate(index, 1, -1);
                }
                Score::RegisterMove();
            }
        }
    }

    bool Game::CheckSolution(Tiling::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        std::vector<Tiling::Index> indices;
        indices.push_back({q, r, s});
        indices.push_back({q + 1, r, s - 1});
        indices.push_back({q + 1, r - 1, s});
        indices.push_back({q - 1, r + 1, s});
        indices.push_back({q - 1, r, s + 1});
        indices.push_back({q, r + 1, s - 1});
        indices.push_back({q, r - 1, s + 1});

        return gameGrid_.CheckEquality(indices);
    }

    void Game::ShuffleSolution(Tiling::Index index)
    {
        int16_t q = std::get<0>(index);
        int16_t r = std::get<1>(index);
        int16_t s = std::get<2>(index);

        std::vector<Tiling::Index> indices;
        indices.push_back({q, r, s});
        indices.push_back({q + 1, r, s - 1});
        indices.push_back({q + 1, r - 1, s});
        indices.push_back({q - 1, r + 1, s});
        indices.push_back({q - 1, r, s + 1});
        indices.push_back({q, r + 1, s - 1});
        indices.push_back({q, r - 1, s + 1});

        gameGrid_.Randomize(indices);
    }

} // namespace hexvoid