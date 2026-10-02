#include "cpu.h"

#include <bits/stdc++.h>
#include <thread>
#include <chrono>

using namespace std;

#define ll long long

vector<pair<ll, ll>> getCPUStats() {
    ifstream file("/proc/stat");
    vector<pair<ll, ll>> stats;
    string line;

    while(getline(file, line)) {
        if(line.rfind("cpu", 0) != 0)
            break;
        stringstream ss(line);
        string cpu;
        ll user, nice, systemTime, idle, iowait, irq, softirq, steal;
        ss >> cpu >> user >> nice >> systemTime >> idle >> iowait >> irq >> softirq >> steal;

        ll idleTime = idle + iowait;
        ll totalTime = user + nice + systemTime + idle + iowait + irq + softirq + steal;
        stats.push_back(make_pair(idleTime, totalTime));
    }
    return stats;
}

void printCPUUsage() {
    vector<pair<ll, ll>> start = getCPUStats();
    if(start.empty()) {
        cout << "CPU Usage: unavailable\n";
        return;
    }

    this_thread::sleep_for(chrono::seconds(1));
    vector<pair<ll, ll>> end = getCPUStats();
    if(end.size() != start.size()) {
        cout << "CPU Usage: unavailable\n";
        return;
    }

    ll idleDiff = end[0].first - start[0].first;
    ll totalDiff = end[0].second - start[0].second;
    if(totalDiff <= 0) {
        cout << "CPU Usage: unavailable\n";
        return;
    }

    double usage = (100.0 * (totalDiff - idleDiff)) / totalDiff;
    cout<<"CPU Usage: "<<usage<<"%\n";
    for(int i=1; i<end.size(); i++) {
        ll coreIdleDiff = end[i].first - start[i].first;
        ll coreTotalDiff = end[i].second - start[i].second;
        if((coreTotalDiff <= 0) || (coreTotalDiff == coreIdleDiff)) continue;
        double coreUsage = (100.0 * (coreTotalDiff - coreIdleDiff)) / coreTotalDiff;
        cout<<"\tCPU"<<i<<": "<<coreUsage<<"%\n";
    }
}