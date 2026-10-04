#pragma once

#include "alerts.h"

struct ResourceValues {
    double cpuUsage;
    double memoryUsage;
    double diskUsage;
};
ResourceValues allSystemInfo();

void runSystemCommand(bool watch, int refresh, const AlertSettings &settings);