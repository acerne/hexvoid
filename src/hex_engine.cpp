
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

    bool Core::animationActive_ = false;

    void Core::SDL(int error)
    {
        if(error < 0) throw std::runtime_error("SDL failed: " + std::string(SDL_GetError()));
    }

    std::mutex Engine::renderMutex_;
    std::condition_variable Engine::renderCondition_;
    bool Engine::renderReady_ = false;
    bool Engine::drawingReady_ = true;

    Engine::GameState Engine::state_;

    std::thread Engine::inputThread_;
    std::thread Engine::displayThread_;

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
        displayThread_ = std::thread(Engine::PresentingThread);

        state_ = GameState::LOAD;
    }

    void Engine::Terminate()
    {
        inputThread_.join();
        displayThread_.join();

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

    bool Engine::GetQuitFlag()
    {
        return quit_;
    }

    Engine::GameState Engine::GetGameState()
    {
        return state_;
    }

    void Engine::SetGameState(Engine::GameState state)
    {
        state_ = state;
        animationActive_ = false;
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

    void Engine::WaitDisplay()
    {
        std::unique_lock<std::mutex> lock(renderMutex_);
        while(!drawingReady_)
            renderCondition_.wait(lock);

        drawingReady_ = false;
    }

    void Engine::ReadyToDisplay()
    {
        std::unique_lock<std::mutex> lock(renderMutex_);
        renderReady_ = true;
        renderCondition_.notify_all();
    }

    void Engine::PresentingThread()
    {
        auto lastRender = std::chrono::system_clock::now();
        int64_t usFrameDuration = 1e6 / 60;
        while(!quit_)
        {
            std::unique_lock<std::mutex> lock(renderMutex_);
            while(!renderReady_)
                renderCondition_.wait(lock);

            auto now = std::chrono::system_clock::now();
            int64_t usElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastRender).count();
            if(usElapsed < usFrameDuration)
                std::this_thread::sleep_for(std::chrono::microseconds(usFrameDuration - usElapsed));

            SDL_RenderPresent(gRenderer_);
            lastRender = std::chrono::system_clock::now();
            Palette::Color c = Palette::GetColor(Palette::Element::Background);
            SDL(SDL_SetRenderDrawColor(gRenderer_, c.r, c.g, c.b, 255));
            SDL(SDL_RenderClear(gRenderer_));

            renderReady_ = false;
            drawingReady_ = true;
            renderCondition_.notify_all();
        }
    }

} // namespace hex
