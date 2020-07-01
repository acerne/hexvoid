
#include "hexvoid.hpp"

/*
 * GLOBAL VARIABLES
 */

// The window we'll be rendering to

// The surface contained by the window

// The image we will load and show on the screen

int main(int argc, char* args[])
{
    SDL_Window* gWindow = NULL;
    SDL_Surface* gSurface = NULL;
    SDL_Surface* gBackground = NULL;
    SDL_Renderer* gRenderer = NULL;

    // Start up SDL and create window
    hexvoid::Initialize(gWindow, gSurface);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    hexvoid::Hexagon hexagon;
    hexagon.x = 100;
    hexagon.y = 100;
    hexagon.radius = 40;
    hexagon.color = 1;

    SDL_RenderClear(gRenderer);

    for(int i = 0; i < 10; i++)
    {
        hexvoid::DrawHexagon(gRenderer, hexagon);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);

        hexagon.x += 30;
        hexagon.y += 30;
        hexagon.color++;
        if(hexagon.color > 4) hexagon.color = 1;
    }
    // Wait two seconds
    SDL_Delay(2000);

    // Free resources and close SDL

    SDL_FreeSurface(gSurface);
    gSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();

    return 0;
}
