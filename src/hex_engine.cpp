
#include <stdexcept>

#include "hex.hpp"

namespace hex
{

    FC_Font* Core::font_ = NULL;
    SDL_Window* Core::gWindow_ = NULL;
    SDL_Surface* Core::gSurface_ = NULL;
    SDL_Surface* Core::gBackground_ = NULL;
    SDL_Renderer* Core::gRenderer_ = NULL;
    uint16_t Core::windowWidth_ = 0;
    uint16_t Core::windowHeight_ = 0;
    bool Core::quit_ = false;
    const char* Core::fontPath_ = "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf";

    void Core::SDL(int error)
    {
        if(error < 0) throw std::runtime_error("SDL failed: " + std::string(SDL_GetError()));
    }

    Engine::GameState Engine::state_;
    std::thread Engine::inputThread_;

    void Engine::Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight)
    {
        windowWidth_ = windowWidth;
        windowHeight_ = windowHeight;

        SDL(SDL_Init(SDL_INIT_VIDEO));

        SDL(TTF_Init());

        gWindow_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                    windowHeight, SDL_WINDOW_SHOWN);
        if(gWindow_ == NULL)
            throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));

        gSurface_ = SDL_GetWindowSurface(gWindow_);
        gRenderer_ = SDL_CreateRenderer(gWindow_, -1, SDL_RENDERER_ACCELERATED);

        font_ = FC_CreateFont();
        FC_LoadFont(font_, gRenderer_, fontPath_, 36, {255, 255, 255, 255}, TTF_STYLE_NORMAL);

        inputThread_ = std::thread(Input::PollingThread);

        state_ = GameState::MAIN_MENU;
    }

    void Engine::Terminate()
    {
        inputThread_.join();

        SDL_FreeSurface(gSurface_);
        gSurface_ = NULL;

        SDL_FreeSurface(gBackground_);
        gBackground_ = NULL;

        SDL_DestroyRenderer(gRenderer_);
        gRenderer_ = NULL;

        SDL_DestroyWindow(gWindow_);
        gWindow_ = NULL;

        FC_FreeFont(font_);

        TTF_Quit();
        SDL_Quit();
    }

    Engine::GameState Engine::GetGameState()
    {
        return state_;
    }

    void Engine::SetGameState(Engine::GameState state)
    {
        state_ = state;
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
        if(quit_) Terminate();

        Palette::Color c = Palette::GetColor(Palette::Element::Background);
        SDL(SDL_SetRenderDrawColor(gRenderer_, c.r, c.g, c.b, 255));
        SDL(SDL_RenderClear(gRenderer_));
    }

    void Engine::Display()
    {
        SDL_RenderPresent(gRenderer_);
    }

} // namespace hex
