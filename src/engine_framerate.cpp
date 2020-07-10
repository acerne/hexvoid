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

    void Framerate::Draw(SDL_Renderer*& gRenderer, const Palette& palette)
    {
        char const* fpsText = std::to_string(fps_).c_str();
        Palette::Color color = palette.GetThemeColor(1);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font_, fpsText, {color.r, color.g, color.b});
        SDL_Rect textLocation = {5, 5, 30, 20};
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, Message, NULL, &textLocation);
    };

} // namespace hexvoid
