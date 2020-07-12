
#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    SDL_Window* Engine::gWindow_ = NULL;
    SDL_Surface* Engine::gSurface_ = NULL;
    SDL_Surface* Engine::gBackground_ = NULL;
    SDL_Renderer* Engine::gRenderer_ = NULL;
    uint16_t Engine::windowWidth_ = 0;
    uint16_t Engine::windowHeight_ = 0;
    bool Engine::debug_ = false;

    void Engine::EnableDebug()
    {
        debug_ = true;
    }

    void Engine::Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight)
    {
        windowWidth_ = windowWidth;
        windowHeight_ = windowHeight;

        if(debug_) printf("Initializing SDL...\n");
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));

        if(debug_) printf("Initializing TTF...\n");
        if(TTF_Init() < 0)
            throw std::runtime_error("SDL_ttf could not initialize! TTF_Error: " + std::string(TTF_GetError()));

        if(debug_) printf("Creating window...\n");
        gWindow_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                    windowHeight, SDL_WINDOW_SHOWN);
        if(gWindow_ == NULL)
            throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));

        if(debug_) printf("Creating renderer...\n");
        gSurface_ = SDL_GetWindowSurface(gWindow_);
        gRenderer_ = SDL_CreateRenderer(gWindow_, -1, SDL_RENDERER_ACCELERATED);
    }

    void Engine::Terminate()
    {
        if(debug_) printf("Terminating SDL...\n");
        SDL_FreeSurface(gSurface_);
        gSurface_ = NULL;

        if(debug_) printf("Terminating TTF...\n");
        SDL_DestroyWindow(gWindow_);
        gWindow_ = NULL;

        if(debug_) printf("Quitting...\n");
        TTF_Quit();
        SDL_Quit();
    }

    void Engine::Clear()
    {
        // hex::Palette::Color background = palette_.GetThemeColor(0);
        if(debug_) printf("Clearing...\n");
        SDL_SetRenderDrawColor(gRenderer_, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer_);
    }

    void Engine::Display()
    {
        if(debug_) printf("Displaying...\n");
        SDL_RenderPresent(gRenderer_);
    }

} // namespace hex
