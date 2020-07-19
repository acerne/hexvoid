
#include <stdexcept>

#include "hex.hpp"

namespace hex
{
    uint16_t Input::cursorX = 0;
    uint16_t Input::cursorY = 0;
    std::mutex Input::clickMutex_;
    std::mutex Input::keyMutex_;
    std::mutex Input::mouseMutex_;
    std::queue<SDL_Event> Input::clicks_;
    std::queue<SDL_Event> Input::keys_;

    void Input::PollingThread()
    {
        SDL_Event event;
        while(!quit_)
        {
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                        AddKeypress(event);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        AddClick(event);
                        break;
                    case SDL_MOUSEMOTION:
                        UpdateMouse(event);
                        break;
                    case SDL_QUIT:
                        quit_ = true;
                        break;
                    default:
                        break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void Input::AddClick(const SDL_Event& event)
    {
        std::lock_guard<std::mutex> guard(clickMutex_);
        clicks_.push(event);
    }

    void Input::AddKeypress(const SDL_Event& event)
    {
        std::lock_guard<std::mutex> guard(keyMutex_);
        keys_.push(event);
    }

    void Input::UpdateMouse(const SDL_Event& event)
    {
        std::lock_guard<std::mutex> guard(mouseMutex_);
        cursorX = event.motion.x;
        cursorY = event.motion.y;
    }

    SDL_Event Input::GetClick()
    {
        std::lock_guard<std::mutex> guard(clickMutex_);
        SDL_Event returnEvent{};
        if(!clicks_.empty())
        {
            returnEvent = clicks_.front();
            clicks_.pop();
        }
        return returnEvent;
    }

    SDL_Event Input::GetKeypress()
    {
        std::lock_guard<std::mutex> guard(keyMutex_);
        SDL_Event returnEvent{};
        if(!keys_.empty())
        {
            returnEvent = keys_.front();
            keys_.pop();
        }
        return returnEvent;
    }

} // namespace hex
