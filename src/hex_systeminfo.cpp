
#include "hex.hpp"
#include "sys/sysinfo.h"
#include "sys/types.h"

namespace hex
{

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

    int getAppVirtMem()
    { // Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while(fgets(line, 128, file) != NULL)
        {
            if(strncmp(line, "VmSize:", 7) == 0)
            {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    int getAppPhysMem()
    { // Note: this value is in KB!
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

    void SystemInfo::Draw()
    {
        struct sysinfo memInfo;

        sysinfo(&memInfo);
        long long totalVirtualMem = memInfo.totalram;
        // Add other values in next statement to avoid int overflow on right hand side...
        totalVirtualMem += memInfo.totalswap;
        totalVirtualMem *= memInfo.mem_unit;

        long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
        // Add other values in next statement to avoid int overflow on right hand side...
        virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
        virtualMemUsed *= memInfo.mem_unit;

        long long totalPhysMem = memInfo.totalram;
        // Multiply in next statement to avoid int overflow on right hand side...
        totalPhysMem *= memInfo.mem_unit;

        long long physMemUsed = memInfo.totalram - memInfo.freeram;
        // Multiply in next statement to avoid int overflow on right hand side...
        physMemUsed *= memInfo.mem_unit;

        const char* formattedInfo =
            "Virtual memory:\n Available: %i MB\n Used: %i MB\n Game: %i MB\n\nPhysical memory:\n "
            "Available: %i MB\n Used: %i MB\n Game: %i MB";

        FC_SetDefaultColor(font_, {255, 255, 255, 255});
        FC_DrawScale(font_, gRenderer_, 5, 30, {0.3, 0.3}, formattedInfo, bToMb(totalVirtualMem), bToMb(virtualMemUsed),
                     kbToMb(getAppVirtMem()), bToMb(totalPhysMem), bToMb(physMemUsed), kbToMb(getAppPhysMem()));
    };

} // namespace hex
