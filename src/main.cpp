#include <chrono>
#include <iostream>

#include "hex.hpp"
#include "hexagon.hpp"
#include "hexvoid.hpp"

int main(int argc, char* args[])
{

    hex::Engine::Initialize("Hexvoid", 800, 600);
    hex::Menu::Initialize();
    hexvoid::Game::Initialize();

    hex::Logo logotype{"HEXVOID", {400, 300}, 8};
    hex::Logo gameOver{"GAME OVER", {400, 300}, 6};
    int64_t splasScreenDuration = 3000;

    hex::Framerate fps;

    auto startTime = std::chrono::system_clock::now();

    while(!hex::Engine::GetQuitFlag())
    {
        fps.Tick();

        hex::Menu::Update();
        hexvoid::Game::Update();

        hex::Engine::Clear();
        // switch(state)
        // {
        //     case GameState::SPLASH_SCREEN:
        //         logotype.Draw();
        //         break;
        //     case GameState::GAME_OVER:
        //         gameOver.Draw();
        //         break;
        //     default:
        //         break;
        // }
        fps.Draw();
        hexvoid::Game::Draw();
        hex::Menu::Draw();
        hex::SystemInfo::Draw();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}