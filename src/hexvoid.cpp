
#include "hexvoid.hpp"

#include <stdexcept>

void hexvoid::Initialize(SDL_Window*& gWindow, SDL_Surface*& gSurface)
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));

    // Create window
    gWindow = SDL_CreateWindow("HexVoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hexvoid::WINDOW_WIDTH,
                               hexvoid::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL)
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));

    // Set window surface
    gSurface = SDL_GetWindowSurface(gWindow);
}

void hexvoid::LoadMedia(const std::string& fileName, SDL_Surface*& gBackground)
{
    // Load splash image
    gBackground = SDL_LoadBMP(fileName.c_str());

    if(gBackground == NULL)
        throw std::runtime_error("Unable to load image " + fileName + " SDL Error: " + std::string(SDL_GetError()));
}
