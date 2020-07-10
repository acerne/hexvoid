
#include "hexvoid.hpp"

int main(int argc, char* args[])
{
    SDL_Window* gWindow = NULL;
    SDL_Surface* gSurface = NULL;
    SDL_Surface* gBackground = NULL;
    SDL_Renderer* gRenderer = NULL;

    // Start up SDL and create window
    hexvoid::Initialize(gWindow, gSurface);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    hexvoid::Cluster cluster(5, 30, 3);

    SDL_Event event;
    bool quit = false;
    int hoverIndex = 0;
    hexvoid::Framerate fps;
    hexvoid::Palette palette;
    hexvoid::Palette::Color background = palette.GetThemeColor(0);
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
                            background = palette.GetThemeColor(0);
                            break;
                        case SDLK_DOWN:
                            palette.PreviousTheme();
                            background = palette.GetThemeColor(0);
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
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(gRenderer, background.r, background.g, background.b, 255);
        SDL_RenderClear(gRenderer);

        cluster.Draw(gRenderer, palette, cursorX, cursorY);
        fps.Draw(gRenderer);

        SDL_RenderPresent(gRenderer);
    }

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    SDL_Quit();

    return 0;
}
