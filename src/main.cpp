#include <chrono>
#include <iostream>

#include "hex.hpp"
#include "hexagon.hpp"

int main(int argc, char* args[])
{

    hex::Engine::Initialize("Hexvoid", 800, 600);
    hex::Menu::Initialize();

    hex::Logo logotype{"HEXVOID", {400, 300}, 8};
    hex::Logo gameOver{"GAME OVER", {400, 300}, 6};
    int64_t splasScreenDuration = 3000;

    hex::Grid grid(3);

    bool quit = false;
    int hoverIndex = 0;
    hex::Framerate fps;
    int16_t cursorX = 0;
    int16_t cursorY = 0;

    auto startTime = std::chrono::system_clock::now();

    while(!quit)
    {
        fps.Tick();
        // while(SDL_PollEvent(&event))
        // {
        //     switch(event.type)
        //     {
        //         case SDL_KEYDOWN:
        //             switch(event.key.keysym.sym)
        //             {
        //                 case SDLK_ESCAPE:
        //                     state = GameState::MAIN_MENU;
        //                     break;
        //                 case SDLK_UP:
        //                     break;
        //                 case SDLK_DOWN:
        //                     break;
        //                 default:
        //                     break;
        //             }
        //             break;
        //         case SDL_MOUSEBUTTONDOWN:
        //             switch(state)
        //             {
        //                 case GameState::GAME:
        //                     switch(event.button.button)
        //                     {
        //                         case SDL_BUTTON_LEFT:
        //                             grid.RotateClockwise(event.motion.x, event.motion.y);
        //                             break;
        //                         case SDL_BUTTON_RIGHT:
        //                             grid.RotateCounterClockwise(event.motion.x, event.motion.y);
        //                             break;
        //                         default:
        //                             break;
        //                     }
        //                     if(hex::Score::IsGameOver())
        //                     {
        //                         startTime = std::chrono::system_clock::now();
        //                         state = GameState::GAME_OVER;
        //                     }
        //                     break;
        //                 case GameState::GAME_OVER:
        //                     /* code */
        //                     break;

        //                 default:
        //                     break;
        //             }
        //             break;
        //         case SDL_QUIT:
        //             quit = true;
        //             break;
        //         default:
        //             break;
        //     }
        // }

        // if(state == GameState::SPLASH_SCREEN || state == GameState::GAME_OVER)
        // {
        //     auto now = std::chrono::system_clock::now();
        //     int64_t msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        //     if(msElapsed > splasScreenDuration) state = GameState::MAIN_MENU;
        // }

        hex::Menu::Update();

        hex::Engine::Clear();
        // switch(state)
        // {
        //     case GameState::SPLASH_SCREEN:
        //         logotype.Draw();
        //         break;
        //     case GameState::GAME:
        //         grid.Draw(cursorX, cursorY);
        //         hex::Score::Draw();
        //         break;
        //     case GameState::GAME_OVER:
        //         gameOver.Draw();
        //         break;
        //     default:
        //         break;
        // }
        // fps.Draw();
        hex::Menu::Draw();
        hex::SystemInfo::Draw();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}