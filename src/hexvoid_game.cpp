#include "hexvoid.hpp"

using namespace hex;

namespace hexvoid
{
    HexagonalGrid Game::gameGrid_;
    Splash Game::titleSplash_;
    Splash Game::gameOverSplash_;
    Twinkle Game::background_;

    std::chrono::system_clock::time_point Game::startTime_;
    int64_t Game::splashDuration_ = 3000;

    void Game::Initialize()
    {
        gameGrid_ = HexagonalGrid{9};
        titleSplash_ = Splash{"HEXVOID", {400, 300}, 8};
        gameOverSplash_ = Splash{"GAME OVER", {400, 300}, 8};
        background_ = Twinkle(21, 15, 20);
    }

    void Game::Update()
    {
        background_.UpdatePhysics();
        switch(Engine::GetGameState())
        {
            case Engine::GameState::LOAD: {
                startTime_ = std::chrono::system_clock::now();
                Engine::SetGameState(Engine::GameState::SPLASH_SCREEN);
                break;
            }
            case Engine::GameState::SPLASH_SCREEN:
            case Engine::GameState::GAME_OVER: {
                auto now = std::chrono::system_clock::now();
                int64_t msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count();
                if(msElapsed > splashDuration_) Engine::SetGameState(Engine::GameState::MAIN_MENU);
                break;
            }
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
                if(Score::IsGameOver())
                {
                    startTime_ = std::chrono::system_clock::now();
                    Engine::SetGameState(Engine::GameState::GAME_OVER);
                }
                gameGrid_.UpdatePhysics();
                break;
            }
            default:
                break;
        }
    }

    void Game::Draw()
    {
        background_.Draw();
        switch(Engine::GetGameState())
        {
            case Engine::GameState::SPLASH_SCREEN:
                titleSplash_.Draw();
                break;
            case Engine::GameState::GAME:
                gameGrid_.Draw(Input::cursorX, Input::cursorY);
                Score::Draw();
                break;
            case Engine::GameState::GAME_OVER:
                gameOverSplash_.Draw();
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
                    gameGrid_.RotateWithMotion(index, 1, -1);
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
                    gameGrid_.RotateWithMotion(index, 1, 1);
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