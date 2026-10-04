#include "alerts.h"
#include "system.h"

#include <bits/stdc++.h>

using namespace std;

string getTimestamp() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    stringstream ss;
    ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void handleResourceLogging(const ResourceValues &stats, const AlertSettings &settings) {
    vector<string> alerts;

    if(settings.cpuLimit >= 0 && stats.cpuUsage >= 0 && stats.cpuUsage >= settings.cpuLimit) {
        stringstream ss;
        ss<<"CPU usage "<<stats.cpuUsage<<"% exceeded limit "<<settings.cpuLimit<<"%";
        alerts.push_back(ss.str());
    }
    if(settings.memoryLimit >= 0 && stats.memoryUsage >= 0 && stats.memoryUsage >= settings.memoryLimit) {
        stringstream ss;
        ss<<"Memory usage "<<stats.memoryUsage<<"% exceeded limit "<<settings.memoryLimit<<"%";
        alerts.push_back(ss.str());
    }
    if(settings.diskLimit >= 0 && stats.diskUsage >= 0 && stats.diskUsage >= settings.diskLimit) {
        stringstream ss;
        ss<<"Disk usage "<<stats.diskUsage<<"% exceeded limit "<<settings.diskLimit<<"%";
        alerts.push_back(ss.str());
    }

    if(!alerts.empty()) {
        cout<<"\nALERTS:\n";
        for(string alert : alerts)
            cout<<"[ALERT] "<<alert<<'\n';
    }

    if(settings.logFile.empty()) return;
    ofstream log(settings.logFile, ios::app);
    if(!log) {
        cout<<"Unable to open log file: "<<settings.logFile<<'\n';
        return;
    }

    log<<getTimestamp()<<" | ";
    if(stats.cpuUsage >= 0)
        log<<"CPU="<<stats.cpuUsage<<"% | ";
    if(stats.memoryUsage >= 0)
        log<<"MEMORY="<<stats.memoryUsage<<"% | ";
    if(stats.diskUsage >= 0)
        log<<"DISK="<<stats.diskUsage<<"%";
    log << '\n';
    for(string alert : alerts)
        log<<getTimestamp()<<" | ALERT | "<<alert<<'\n';
}