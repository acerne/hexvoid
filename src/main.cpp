
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

    // Load media
    hexvoid::LoadMedia("image.bmp", gBackground);

    // Apply the image
    SDL_BlitSurface(gBackground, NULL, gSurface, NULL);
    // Update the surface
    SDL_UpdateWindowSurface(gWindow);

    SDL_Delay(1000);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect rect;

    rect.x = 250;
    rect.y = 150;
    rect.w = 200;
    rect.h = 200;

    for(int i = 0; i < 100; i++)
    {
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0, 255, 127, 255);
        SDL_RenderDrawRect(gRenderer, &rect);

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);

        rect.x++;
        rect.y--;
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
