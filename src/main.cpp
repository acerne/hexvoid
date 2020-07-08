
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

    std::vector<hexvoid::Hexagon> hexGrid = hexvoid::GenerateHexagonGrid(5, 30, 3);

    SDL_Event event;
    bool quit = false;
    hexvoid::Hexagon hover;
    hexvoid::FPS frameRate;
    hexvoid::Palette palette;

    while(!quit)
    {
        frameRate.Tick();
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
                    hover = hexvoid::FindClosestHexagon(hexGrid, event.motion.x, event.motion.y);
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        SDL_RenderClear(gRenderer);

        for(auto hex : hexGrid)
        {
            hexvoid::Palette::Color color = palette.GetThemeColor(hex.family);
            hexvoid::DrawVerticalHexagon(gRenderer, hex, color.r, color.g, color.b);
        }

        hexvoid::Palette::Color blank = palette.GetThemeColor(0);
        hexvoid::DrawVerticalHexagon(gRenderer, hover, blank.r, blank.g, blank.b);

        frameRate.Draw(gRenderer);

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
