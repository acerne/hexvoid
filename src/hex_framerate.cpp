
#include "hex.hpp"

namespace hex
{
    Framerate::Framerate()
    {
        frameCount_ = 0;
        lastSecond_ = SDL_GetTicks();
        fps_ = 0;
        text_.Place(5, 5);
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
        text_.Set(std::to_string(fps_));
        text_.Draw();
    };

} // namespace hex
