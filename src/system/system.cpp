#include "cpu.h"
#include "network.h"
#include "system.h"
#include "../util.h"

#include <bits/stdc++.h>
#include <chrono>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <thread>
#include <unistd.h>

using namespace std;

#define ll long long

string getHostname() {
    char hostname[64];
    if(gethostname(hostname, sizeof(hostname)) != 0)
        return "unavailable";
    return hostname;
}

void printSystemInfo() {
    utsname info;
    if(uname(&info) != 0) {
        cout<<"Unable to retrieve system information.\n";
        return;
    }
    else cout<<"Hostname: "<<getHostname()<<'\n';
    cout<<"Kernel: "<<info.release<<'\n';
    cout<<"Architecture: "<<info.machine<<'\n';
}

void printUptime() {
    ifstream file("/proc/uptime");
    double uptime;
    if(!(file >> uptime)) {
        cout<<"Uptime: unavailable\n";
        return;
    }
    ll sec = (ll)(uptime);
    ll hr = sec/3600;
    ll min = (sec%3600)/60;
    cout<<"Uptime: "<<hr<<"h "<<min<<"m\n";
}

void printLoadAverage() {
    ifstream file("/proc/loadavg");
    double one, five, fifteen;
    if(!(file >> one >> five >> fifteen)) {
        cout<<"Load Average : unavailable\n";
        return;
    }
    cout<<"Load Average : "<<one<<" "<<five<<" "<<fifteen<<'\n';
}

void printMemory() {
    ifstream file("/proc/meminfo");
    string key, unit;
    ll value, total = 0, available = 0;
    while(file >> key >> value >> unit) {
        if(key=="MemTotal:") total = value;
        else if(key=="MemAvailable:") available = value;
    }
    if(total == 0) {
        cout<<"Memory: unavailable\n";
        return;
    }
    ll used = total - available;
    cout<<"Memory: "<<(used/1024)<<" MB / "<<(total/1024)<<" MB\n";
}

void printSwap() {
    ifstream file("/proc/meminfo");
    string key, unit;
    ll value, total = 0, free = 0;

    while(file >> key >> value >> unit) {
        if(key == "SwapTotal:")
            total = value;
        else if(key == "SwapFree:")
            free = value;
    }
    if(total == 0) {
        cout<<"Swap: unavailable\n";
        return;
    }

    ll used = total - free;
    cout<<"Swap: "<<used/1024<<" MB / "<<total/1024<<" MB\n";
}

void printDisk() {
    struct statvfs stats;
    if(statvfs("/", &stats) != 0) {
        cout<<"Disk Usage: unavailable\n";
        return;
    }
    ll total = (ll)(stats.f_blocks) * stats.f_frsize;
    ll available = (ll)(stats.f_bavail) * stats.f_frsize;
    ll used = total - available;
    double usage = (100.0 * used)/total;
    cout<<"Disk Usage: "<<usage<<"%\n";
}

void printDiskIO() {
    ifstream file("/proc/diskstats");
    if(!file) {
        cout<<"Disk I/O: unavailable\n";
        return;
    }

    string line;
    ll totalRead = 0, totalWrite = 0;
    while(getline(file, line)) {
        stringstream ss(line);
        ll temp;
        string device;
        ss >> temp >> temp >> device;
        string partitionPath = "/sys/class/block/" + device + "/partition";
        ifstream partition(partitionPath);
        if(partition) continue;

        ll sectorsRead, sectorsWritten;
        ss >> temp >> temp >> sectorsRead >> temp >> temp >> sectorsWritten;
        totalRead += sectorsRead;
        totalWrite += sectorsWritten;
    }

    totalRead *= 512;
    totalWrite *= 512;
    cout<<"Disk Read: "<<formatBytes(totalRead)<<"\n";
    cout<<"Disk Write: "<<formatBytes(totalWrite)<<"\n";
}

void allSystemInfo(){
    cout<<"SYSTEM: \n";
    printSystemInfo();
    printUptime();
    printLoadAverage();

    cout<<"\nRESOURCES: \n";
    printMemory();
    printSwap();
    printDisk();
    printNetworkTraffic();
    printCPUUsage();
}

void runSystemCommand(bool watch, int refresh) {
    if(!watch){
        allSystemInfo();
        return;
    }

    while(true){
        clearScreen();
        allSystemInfo();
        cout<<"\nPress Ctrl+C to exit watch mode.\n";
        if(refresh>1)
            this_thread::sleep_for(chrono::seconds(refresh - 1));
    }
}