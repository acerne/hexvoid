
#include "hex.hpp"
#include "sys/sysinfo.h"
#include "sys/types.h"

namespace hex
{
    uint16_t SystemInfo::frameCount_ = 0;
    uint32_t SystemInfo::lastSecond_ = 0;
    uint16_t SystemInfo::fps_ = 0;
    uint16_t SystemInfo::ram_ = 0;

    int parseLine(char* line)
    {
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char* p = line;
        while(*p < '0' || *p > '9')
            p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
    }

    int getAppPhysMem()
    {
        // Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while(fgets(line, 128, file) != NULL)
        {
            if(strncmp(line, "VmRSS:", 6) == 0)
            {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    inline long long bToMb(long long b)
    {
        return b / 1048576;
    }

    inline long long kbToMb(long long kb)
    {
        return kb / 1024;
    }

    void SystemInfo::Update()
    {
        uint32_t tick = SDL_GetTicks();
        frameCount_++;
        if(tick - lastSecond_ > 1000)
        {
            fps_ = frameCount_;
            lastSecond_ = tick;
            frameCount_ = 0;
            ram_ = getAppPhysMem() / 1024;
        }
    };

    void SystemInfo::Draw()
    {
        if(fps_ < 30)
            FC_SetDefaultColor(font_, {255, 0, 0, 255});
        else
            FC_SetDefaultColor(font_, {0, 255, 0, 255});

        FC_DrawScale(font_, gRenderer_, 5, 5, {0.3, 0.3}, "FPS %i", fps_);

        if(ram_ > 100)
            FC_SetDefaultColor(font_, {255, 0, 0, 255});
        else
            FC_SetDefaultColor(font_, {0, 255, 0, 255});

        FC_DrawScale(font_, gRenderer_, 5, 30, {0.3, 0.3}, "RAM %i MB", ram_);
    };

} // namespace hex
