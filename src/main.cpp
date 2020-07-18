#include <chrono>
#include <iostream>

#include "hex.hpp"

enum class GameState
{
    SPLASH_SCREEN,
    MAIN_MENU,
    NEW_GAME_MENU,
    OPTIONS_MENU,
    GAME,
    GAME_OVER
};

int main(int argc, char* args[])
{
    // hex::Engine::EnableDebug();

    hex::Engine::Initialize("Hexvoid", 800, 600);

    GameState state = GameState::SPLASH_SCREEN;

    hex::Logo logotype{};
    int64_t splasScreenDuration = 3000;

    hex::Menu mainMenu(60);
    mainMenu.AddItem("New game", 1);
    mainMenu.AddItem("Options", 2);
    mainMenu.AddItem("Exit", 3);

    hex::Menu newGameMenu(60);
    newGameMenu.AddOption("Grid size:", 1, {"7", "9", "11"});
    newGameMenu.AddOption("Difficulty:", 2, {"Easy", "Medium", "Hard"});
    newGameMenu.AddItem("Start", 4);
    newGameMenu.AddItem("Back", 6);

    hex::Menu optionsMenu(60);
    optionsMenu.AddOption("Resolution:", 0, {"800x600", "1280x720", "1600x900", "1920x1080"});
    optionsMenu.AddOption("Color theme:", 1, hex::Palette::GetThemeNames());
    optionsMenu.AddItem("Apply", 4);
    optionsMenu.AddItem("Back", 6);

    hex::Grid grid(3);

    SDL_Event event;
    bool quit = false;
    int hoverIndex = 0;
    hex::Framerate fps;
    int16_t cursorX = 0;
    int16_t cursorY = 0;

    auto startTime = std::chrono::system_clock::now();

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
                            state = GameState::MAIN_MENU;
                            break;
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
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
                        case GameState::GAME:
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
                        case GameState::MAIN_MENU:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(mainMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 1:
                                            state = GameState::NEW_GAME_MENU;
                                            break;
                                        case 2:
                                            state = GameState::OPTIONS_MENU;
                                            break;
                                        case 3:
                                            quit = true;
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        case GameState::NEW_GAME_MENU:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(newGameMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 4: {
                                            std::string gridSize = newGameMenu.GetSelection("Grid size:");
                                            grid = hex::Grid(std::stoi(gridSize));
                                            hex::Score::Start();
                                            state = GameState::GAME;
                                            break;
                                        }
                                        case 6:
                                            state = GameState::MAIN_MENU;
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
                        case GameState::OPTIONS_MENU:
                            switch(event.button.button)
                            {
                                case SDL_BUTTON_LEFT: {
                                    switch(optionsMenu.Click(event.motion.x, event.motion.y))
                                    {
                                        case 1: {
                                            std::string theme = optionsMenu.GetSelection("Color theme:");
                                            hex::Palette::ChangeTheme(theme);
                                            break;
                                        }
                                        case 4: {
                                            std::string resolution = optionsMenu.GetSelection("Resolution:");
                                            int x = resolution.find("x");
                                            std::string resolutionX = resolution.substr(0, x);
                                            std::string resolutionY =
                                                resolution.substr(x + 1, resolution.length() - x - 1);
                                            std::cout << resolution << " " << resolutionX << " " << resolutionY
                                                      << std::endl;
                                            hex::Engine::ChangeResolution(std::stoi(resolutionX),
                                                                          std::stoi(resolutionY));
                                            break;
                                        }
                                        case 6:
                                            state = GameState::MAIN_MENU;
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
                        case GameState::GAME_OVER:
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

        if(state == GameState::SPLASH_SCREEN)
        {
            auto now = std::chrono::system_clock::now();
            int64_t msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            if(msElapsed > splasScreenDuration) state = GameState::MAIN_MENU;
        }

        hex::Engine::Clear();
        switch(state)
        {
            case GameState::SPLASH_SCREEN:
                logotype.Draw();
                break;
            case GameState::MAIN_MENU:
                mainMenu.Draw(cursorX, cursorY);
                break;
            case GameState::NEW_GAME_MENU:
                newGameMenu.Draw(cursorX, cursorY);
                break;
            case GameState::OPTIONS_MENU:
                optionsMenu.Draw(cursorX, cursorY);
                break;
            case GameState::GAME:
                grid.Draw(cursorX, cursorY);
                hex::Score::Draw();
                break;
            case GameState::GAME_OVER:
                /* code */
                break;
            default:
                break;
        }
        fps.Draw();
        hex::SystemInfo::Draw();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}
