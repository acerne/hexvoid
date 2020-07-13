#include <iostream>

#include "hex.hpp"

int main(int argc, char* args[])
{
    // hex::Engine::EnableDebug();

    hex::Engine::Initialize("Hexvoid", 800, 600);

    hex::Menu mainMenu(3, 60, 40);
    mainMenu.AddItem("New game", 0);
    mainMenu.AddItem("Options", 1);
    mainMenu.AddItem("Exit", 2);

    hex::Grid grid(4, 50);

    SDL_Event event;
    bool quit = false;
    int hoverIndex = 0;
    hex::Framerate fps;
    hex::SystemInfo info;
    hex::Palette palette;
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
                            quit = true;
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
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }

        hex::Engine::Clear();
        mainMenu.Draw(cursorX, cursorY);
        // grid.Draw(palette, cursorX, cursorY);
        fps.Draw();
        info.Draw();
        // palette.DrawInfo();
        hex::Engine::Display();
    }

    hex::Engine::Terminate();

    return 0;
}
