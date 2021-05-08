#pragma once
#include <windows.h>
#include <list>

/**
* Get and calculate Cpu Load
*
**/
class CpuManager {
private:
    std::list<double> _cpuLoadList;
    ULARGE_INTEGER _oldIdleTime, _oldKernelTime, _oldUserTime;
    bool _initialized;

public:
    CpuManager();

    /**
    * Refresh the value of the cpuLoad
    *
    * @return true if succeed, false otherwise
    **/
    bool refreshCpuLoad();

    /**
    * Get the cpu load value
    *
    * @return value of the cpu load, -1 if cannot be calculated yet
    **/
    double getCpuLoad();

    /**
    * Get the average values of cpuLoad over time
    *
    * @return list of values for the average of 5, 10, 15 sec. -1 if cannot be calculated yet
    **/
    std::list<double> getAverage();

private:
    void addCpuLoadToList(double cpuLoad);

    double calculateAverage(int n);
};