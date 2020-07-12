
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

    SystemInfo::SystemInfo()
    {
        if(debug_) printf("Creating SystemInfo class...\n");

        textInfo_.Place(5, 30);
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

        textInfo_.Set("Virtual memory:", 0);
        textInfo_.Set(" Available : " + std::to_string(bToMb(totalVirtualMem)) + " MB", 1);
        textInfo_.Set(" Used: " + std::to_string(bToMb(virtualMemUsed)) + " MB", 2);
        textInfo_.Set(" Game: " + std::to_string(kbToMb(getAppVirtMem())) + " MB", 3);

        textInfo_.Set("Physical memory:", 4);
        textInfo_.Set(" Available : " + std::to_string(bToMb(totalPhysMem)) + " MB", 5);
        textInfo_.Set(" Used: " + std::to_string(bToMb(physMemUsed)) + " MB", 6);
        textInfo_.Set(" Game: " + std::to_string(kbToMb(getAppPhysMem())) + " MB", 7);

        textInfo_.Draw();
    };

} // namespace hex
