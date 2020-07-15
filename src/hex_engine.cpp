
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
    const char* Engine::fontPath_ = "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf";

    void Engine::Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight)
    {
        windowWidth_ = windowWidth;
        windowHeight_ = windowHeight;

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));

        if(TTF_Init() < 0)
            throw std::runtime_error("SDL_ttf could not initialize! TTF_Error: " + std::string(TTF_GetError()));

        gWindow_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                    windowHeight, SDL_WINDOW_SHOWN);
        if(gWindow_ == NULL)
            throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));

        gSurface_ = SDL_GetWindowSurface(gWindow_);
        gRenderer_ = SDL_CreateRenderer(gWindow_, -1, SDL_RENDERER_ACCELERATED);
    }

    void Engine::Terminate()
    {
        SDL_FreeSurface(gSurface_);
        gSurface_ = NULL;

        SDL_FreeSurface(gBackground_);
        gBackground_ = NULL;

        SDL_DestroyRenderer(gRenderer_);
        gRenderer_ = NULL;

        SDL_DestroyWindow(gWindow_);
        gWindow_ = NULL;

        TTF_Quit();
        SDL_Quit();
    }

    void Engine::ChangeResolution(uint16_t windowWidth, uint16_t windowHeight)
    {
        int16_t xOffset = (windowWidth - windowWidth_) / 2;
        int16_t yOffset = (windowHeight - windowHeight_) / 2;

        windowWidth_ = windowWidth;
        windowHeight_ = windowHeight;

        int x, y;
        SDL_GetWindowPosition(gWindow_, &x, &y);
        SDL_SetWindowPosition(gWindow_, x - xOffset, y - yOffset);
        SDL_SetWindowSize(gWindow_, windowWidth, windowHeight_);
    }

    void Engine::Clear()
    {
        Palette::Color c = Palette::GetColor(Palette::Element::Background);
        SDL_SetRenderDrawColor(gRenderer_, c.r, c.g, c.b, 255);
        SDL_RenderClear(gRenderer_);
    }

    void Engine::Display()
    {
        SDL_RenderPresent(gRenderer_);
    }

} // namespace hex
