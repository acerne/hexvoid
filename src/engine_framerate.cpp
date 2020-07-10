#include <stdexcept>

#include "engine.hpp"

namespace hexvoid
{
    Framerate::Framerate()
    {
        frameCount_ = 0;
        lastSecond_ = SDL_GetTicks();
        fps_ = 0;
        font_ = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 12); // select and move font
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        textColor_ = {255, 255, 127};
    }

    Framerate::~Framerate()
    {
        TTF_CloseFont(font_);
    }

    void Framerate::Tick()
    {
        uint32_t tick = SDL_GetTicks();
        frameCount_++;
        if(tick - lastSecond_ > 1000)
        {
            fps_ = frameCount_;
            lastSecond_ = tick;
            frameCount_ = 0;
        }
    };

    void Framerate::Draw(SDL_Renderer*& gRenderer)
    {
        char const* fpsTexst = std::to_string(fps_).c_str();
        SDL_Surface* textSurface = TTF_RenderText_Solid(font_, fpsTexst, textColor_);
        SDL_Rect textLocation = {5, 5, 30, 20};
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, Message, NULL, &textLocation);
    };

} // namespace hexvoid
