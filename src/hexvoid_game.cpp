#include "hexvoid.hpp"

namespace hexvoid
{
    hex::Grid hexvoid::Game::gameGrid_;

    void Game::Initialize()
    {
        // hex::Grid grid(3);
        gameGrid_ = hex::Grid(9);
    }

    void Game::Update()
    {
        switch(hex::Engine::GetGameState())
        {
            case hex::Engine::GameState::GAME: {
                SDL_Event clickEvent = hex::Input::GetClick();
                switch(clickEvent.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        gameGrid_.RotateClockwise(clickEvent.motion.x, clickEvent.motion.y);
                        break;
                    case SDL_BUTTON_RIGHT:
                        gameGrid_.RotateCounterClockwise(clickEvent.motion.x, clickEvent.motion.y);
                        break;
                    default:
                        break;
                }
                // if(hex::Score::IsGameOver())
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
        switch(hex::Engine::GetGameState())
        {
            case hex::Engine::GameState::GAME:
                gameGrid_.Draw(hex::Input::cursorX, hex::Input::cursorY);
                hex::Score::Draw();
                break;
            default:
                break;
        }
    }

} // namespace hexvoid