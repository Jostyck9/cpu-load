#include <windows.h>
#include <list>
#include "CpuManager.h"

/**
* Display the cpu load and average
*
* @param index Number displayed at the beginning of the line
* @param cpuLoad value of the cpuLoad to display
* @param cpuAverage list of cpu load average by 5, 10 and 15sec
**/
void printCpuLoad(long index, double cpuLoad, std::list<double>& cpuAverage) {
    int i = 1;
    SYSTEMTIME time;

    GetLocalTime(&time);
    printf("%i %04d.%02d.%02d %02d:%02d:%02d :", index, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
        time.wSecond);
    if (cpuLoad != -1)
        printf(" [CPU Load:\t%.2f%%]", cpuLoad);
    for (double value : cpuAverage) {
        if (value != -1)
            printf(" [%isec avg:\t%.2f%%]", i * 5, value);
        i++;
    }
    printf("\n");
}

int main() {
    long index = 0;
    double cpuLoad;
    std::list<double> cpuAverage;
    CpuManager cpu = CpuManager();


    while (1) {
        if (cpu.refreshCpuLoad() == -1)
            return 84;
        cpuLoad = cpu.getCpuLoad();
        cpuAverage = cpu.getAverage();
        printCpuLoad(index, cpuLoad, cpuAverage);
        index++;
        Sleep(1000);
    }
}