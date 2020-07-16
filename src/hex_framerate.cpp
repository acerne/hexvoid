
#include "hex.hpp"

namespace hex
{
    Framerate::Framerate()
    {
        frameCount_ = 0;
        lastSecond_ = SDL_GetTicks();
        fps_ = 0;
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

    void Framerate::Draw()
    {
        FC_SetDefaultColor(Engine::font_, {255, 0, 0, 255});
        FC_DrawScale(Engine::font_, Engine::gRenderer_, 5, 5, {0.5, 0.5}, "FPS %i", fps_);
    };

} // namespace hex
