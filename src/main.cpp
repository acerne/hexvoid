#include <iostream>

#include "hex.hpp"

enum class GameState
{
    MainMenu,
    NewGameMenu,
    OptionsMenu,
    Game,
    GameOver
};

int main(int argc, char* args[])
{
    // hex::Engine::EnableDebug();

    hex::Engine::Initialize("Hexvoid", 800, 600);

    hex::Palette palette;

    hex::Menu mainMenu(60, 40);
    mainMenu.AddItem("New game", 0);
    mainMenu.AddItem("Options", 1);
    mainMenu.AddItem("Exit", 2);

    hex::Menu newGameMenu(60, 40);
    newGameMenu.AddOption("Grid size:", 1, {"7", "9", "11", "13"});
    newGameMenu.AddOption("Difficulty:", 2, {"Easy", "Medium", "Hard"});
    newGameMenu.AddItem("Start", 4);
    newGameMenu.AddItem("Back", 6);

    hex::Menu optionsMenu(60, 40);
    optionsMenu.AddOption("Resolution:", 0, {"800x600", "1920x1080"});
    optionsMenu.AddOption("Color style:", 1, {"A", "B", "C"});
    optionsMenu.AddItem("Back", 6);

    GameState state = GameState::MainMenu;

    hex::Grid grid(4, 50);

    SDL_Event event;
    bool quit = false;
    int hoverIndex = 0;
    hex::Framerate fps;
    hex::SystemInfo info;
    int16_t cursorX = 0;
    int16_t cursorY = 0;

    while(!quit)
    {
        fps.Tick();
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            state = GameState::MainMenu;
                            break;
                        case SDLK_UP:
                            palette.NextTheme();
                            break;
                        case SDLK_DOWN:
                            palette.PreviousTheme();
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_MOUSEMOTION:
                    cursorX = event.motion.x;
                    cursorY = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch(state)
                    {
                        case GameState::Game:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                    grid.RotateClockwise(event.motion.x, event.motion.y);
                                    break;
                                case SDL_BUTTON_RIGHT:
                                    grid.RotateCounterClockwise(event.motion.x, event.motion.y);
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case GameState::MainMenu:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(mainMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 0:
                                            state = GameState::NewGameMenu;
                                            break;
                                        case 1:
                                            state = GameState::OptionsMenu;
                                            break;
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        case GameState::NewGameMenu:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(newGameMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 4: {
                                            std::string gridSize = newGameMenu.GetSelection("Grid size:");
                                            grid = hex::Grid((std::stoi(gridSize) - 1) / 2, 30);
                                            state = GameState::Game;
                                            break;
                                        }
                                        case 6:
                                            state = GameState::MainMenu;
                                            break;
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        case GameState::OptionsMenu:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(optionsMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 6:
                                            state = GameState::MainMenu;
                                            break;
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        case GameState::GameOver:
                            /* code */
                            break;

                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }

        hex::Engine::Clear();
        switch(state)
        {
            case GameState::Game:
                grid.Draw(palette, cursorX, cursorY);
                break;
            case GameState::MainMenu:
                mainMenu.Draw(cursorX, cursorY);
                break;
            case GameState::NewGameMenu:
                newGameMenu.Draw(cursorX, cursorY);
                break;
            case GameState::OptionsMenu:
                optionsMenu.Draw(cursorX, cursorY);
                break;
            case GameState::GameOver:
                /* code */
                break;
            default:
                break;
        }
        fps.Draw();
        info.Draw();
        // palette.DrawInfo();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}
