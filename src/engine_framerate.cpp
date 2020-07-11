#include <stdexcept>

#include "engine.hpp"

namespace hexvoid
{
    Framerate::Framerate()
    {
        frameCount_ = 0;
        lastSecond_ = SDL_GetTicks();
        fps_ = 0;
        text_.Position(5, 5);
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
        text_.Apply(std::to_string(fps_));
        text_.Draw(gRenderer);
    };

} // namespace hexvoid
