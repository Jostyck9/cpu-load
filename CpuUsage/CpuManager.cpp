#include <windows.h>
#include <iostream>
#include "CpuManager.h"

CpuManager::CpuManager() {
    _initialized = false;
    _oldIdleTime = { 0 };
    _oldKernelTime = { 0 };
    _oldUserTime = { 0 };
}

bool CpuManager::refreshCpuLoad() {
    FILETIME idleTime, kernelTime, userTime;
    ULARGE_INTEGER idleTimeLong, kernelTimeLong, userTimeLong;
    ULONGLONG usr, ker, idl, sys;
    double cpuLoad;

    if (GetSystemTimes(&idleTime, &kernelTime, &userTime) <= 0) {
        std::cerr << "An error occurred while reading current CPU load." << std::endl;
        return false;
    }
    idleTimeLong.LowPart = idleTime.dwLowDateTime;
    idleTimeLong.HighPart = idleTime.dwHighDateTime;
    kernelTimeLong.LowPart = kernelTime.dwLowDateTime;
    kernelTimeLong.HighPart = kernelTime.dwHighDateTime;
    userTimeLong.LowPart = userTime.dwLowDateTime;
    userTimeLong.HighPart = userTime.dwHighDateTime;

    if (_initialized) {
        idl = idleTimeLong.QuadPart - _oldIdleTime.QuadPart;

        usr = userTimeLong.QuadPart - _oldUserTime.QuadPart;
        ker = kernelTimeLong.QuadPart - _oldKernelTime.QuadPart;
        sys = ker + usr;

        cpuLoad = (1.0 - ((sys > 0) ? ((double)idl) / sys : 0)) * 100;
        addCpuLoadToList(cpuLoad);
    }
    _oldUserTime = userTimeLong;
    _oldKernelTime = kernelTimeLong;
    _oldIdleTime = idleTimeLong;
    _initialized = true;
    return true;
}

double CpuManager::getCpuLoad() {
    if (_cpuLoadList.size() == 0) {
        return -1;
    }
    return _cpuLoadList.front();
}

std::list<double> CpuManager::getAverage() {
    std::list<double> averages;

    averages.push_back(calculateAverage(5));
    averages.push_back(calculateAverage(10));
    averages.push_back(calculateAverage(15));
    return averages;
}

void CpuManager::addCpuLoadToList(double cpuLoad) {
    _cpuLoadList.push_front(cpuLoad);
    while (_cpuLoadList.size() >= 16)
        _cpuLoadList.pop_back();
}

double CpuManager::calculateAverage(int n) {
    int i = 0;
    double res = 0;

    if (_cpuLoadList.size() < n) {
        return -1;
    }
    for (double value : _cpuLoadList) {
        if (i >= n) {
            break;
        }
        i++;
        res += value;
    }
    return res / n;
}