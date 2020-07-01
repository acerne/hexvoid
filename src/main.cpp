
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

    SDL_RenderClear(gRenderer);

    std::vector<hexvoid::Hexagon> hexGrid = hexvoid::GenerateHexagonGrid(10, 15, 30, 3);

    for(auto hex : hexGrid)
        hexvoid::DrawHexagon(gRenderer, hex);

    SDL_RenderPresent(gRenderer);

    // Wait two seconds
    SDL_Delay(5000);

    // Free resources and close SDL

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();

    return 0;
}
