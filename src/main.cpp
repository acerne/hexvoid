
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

    while(!quit)
    {
        frameRate.Tick();
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE) quit = true;
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
            hexvoid::DrawVerticalHexagon(gRenderer, hex);

        hover.color = 0;
        hexvoid::DrawVerticalHexagon(gRenderer, hover);

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
