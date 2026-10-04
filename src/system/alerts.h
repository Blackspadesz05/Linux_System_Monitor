#pragma once

#include <string>

using namespace std;

struct ResourceValues;
struct AlertSettings {
    double cpuLimit = -1, memoryLimit = -1, diskLimit = -1;
    string logFile;
};

void handleResourceLogging(const ResourceValues &stats, const AlertSettings &settings);