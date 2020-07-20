#include <chrono>
#include <iostream>

#include "hex.hpp"
#include "hexvoid.hpp"

int main(int argc, char* args[])
{

    hex::Engine::Initialize("Hexvoid", 800, 600);
    hex::Menu::Initialize();
    hexvoid::Game::Initialize();

    while(!hex::Engine::GetQuitFlag())
    {
        hex::Menu::Update();
        hexvoid::Game::Update();
        hex::SystemInfo::Update();

        hex::Engine::WaitDisplay();
        hex::Menu::Draw();
        hexvoid::Game::Draw();
        hex::SystemInfo::Draw();
        hex::Engine::ReadyToDisplay();
    }

    hex::Engine::Terminate();

    return 0;
}