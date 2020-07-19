#include <chrono>
#include <iostream>

#include "hex.hpp"
#include "hexvoid.hpp"

int main(int argc, char* args[])
{

    hex::Engine::Initialize("Hexvoid", 800, 600);
    hex::Menu::Initialize();
    hexvoid::Game::Initialize();

    hex::Splash logotype{"HEXVOID", {400, 300}, 8};
    hex::Splash gameOver{"GAME OVER", {400, 300}, 6};
    int64_t splasScreenDuration = 3000;

    auto startTime = std::chrono::system_clock::now();

    while(!hex::Engine::GetQuitFlag())
    {
        hex::Menu::Update();
        hexvoid::Game::Update();
        hex::SystemInfo::Update();

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
        hex::Menu::Draw();
        hexvoid::Game::Draw();
        hex::SystemInfo::Draw();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}