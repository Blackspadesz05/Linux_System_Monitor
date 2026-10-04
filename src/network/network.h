#pragma once

#include <string>
#include <map>

using namespace std;

#define ll long long

struct NetworkInterfaceStats {
    ll rxBytes = 0, rxPackets = 0, rxErrors = 0, rxDropped = 0;
    ll txBytes = 0, txPackets = 0, txErrors = 0, txDropped = 0;
};

struct NetworkValues {
    ll rxBytesPerSec = -1, txBytesPerSec = -1;
};

map<string, NetworkInterfaceStats> getNetworkStats();

NetworkValues printNetworkTraffic(int sampleTime = 1, const string &filter = "");

void runNetworkCommand(bool watch, int refresh, const string &filter = "");