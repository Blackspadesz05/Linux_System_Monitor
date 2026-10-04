#include "process.h"
#include "../util.h"

#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>

using namespace std;
#define ll long long

struct ProcessInfo {
    ll pid, parentPID, memory, threads, cpuTime;
    string name, state;
    double cpuUsage;
};

bool isNumber(string str) {
    if(str.empty())
        return false;
    for(char c : str) {
        if(c < '0' || c > '9')
            return false;
    }
    return true;
}

bool readProcessInfo(string pid, ProcessInfo &process) {
    string path = "/proc/" + pid + "/status";
    ifstream file(path);
    if(!file) return false;

    process.pid = stoll(pid);
    process.name = "unknown";
    process.state = "unknown";
    process.parentPID = 0;
    process.memory = 0;
    process.threads = 0;
    process.cpuTime = 0;
    process.cpuUsage = 0;

    string line;
    while(getline(file, line)) {
        size_t pos = line.find(':');
        if(pos == string::npos) continue;

        string key = line.substr(0, pos), value = line.substr(pos + 1);
        while(!value.empty() && (value[0] == ' ' || value[0] == '\t'))
            value.erase(value.begin());
        if(key == "Name")
            process.name = value;
        else {
            stringstream ss(value);
            if(key == "State")
                ss >> process.state;
            else if(key == "PPid")
                ss >> process.parentPID;
            else if(key == "VmRSS") {
                ll temp; ss >> temp;
                process.memory = temp * 1024;
            }
            else if(key == "Threads")
                ss >> process.threads;
        }
    }
    file.close();

    string statPath = "/proc/" + pid + "/stat";
    ifstream statFile(statPath);
    if(!statFile) return false;
    string statLine;
    if(!getline(statFile, statLine)) return false;

    size_t closingBracket = statLine.rfind(')');
    if(closingBracket == string::npos) return false;
    string data = statLine.substr(closingBracket + 2);
    stringstream ss(data);

    char state;
    ll temp;
    ss >> state;
    for(int i = 0; i < 10; i++) ss >> temp;
    ll userTime, systemTime;
    ss >> userTime >> systemTime;
    process.cpuTime = userTime + systemTime;
    return true;
}

ll getTotalCPUTime() {
    ifstream file("/proc/stat");
    if(!file) return 0;
    string cpu;
    ll user, nice, systemTime, idle, iowait, irq, softirq, steal;
    file >> cpu >> user >> nice >> systemTime >> idle >> iowait >> irq >> softirq >> steal;
    return user + nice + systemTime + idle + iowait + irq + softirq + steal;
}

vector<ProcessInfo> getProcesses() {
    vector<ProcessInfo> processes;
    DIR* directory = opendir("/proc");
    if(!directory) return processes;
    struct dirent* entry;
    while((entry = readdir(directory)) != NULL) {
        string pid = entry->d_name;
        if(!isNumber(pid)) continue;
        ProcessInfo process;
        if(readProcessInfo(pid, process))
            processes.push_back(process);
    }
    closedir(directory);
    return processes;
}

void calculateCPUUsage(vector<ProcessInfo> &processes) {
    vector<ProcessInfo> start = getProcesses();
    ll totalStart = getTotalCPUTime();
    cout << "[Calculating CPU Usage...]\n";

    this_thread::sleep_for(chrono::seconds(1));
    vector<ProcessInfo> end = getProcesses();
    ll totalEnd = getTotalCPUTime();
    ll totalDiff = totalEnd - totalStart;
    if(totalDiff <= 0) return;

    ll cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
    map<int, ll> startCPU;
    for(auto &process : start)
        startCPU[process.pid] = process.cpuTime;
    processes.clear();

    for(auto &process : end) {
        if(startCPU.find(process.pid) == startCPU.end()) continue;
        ll processDiff = process.cpuTime - startCPU[process.pid];
        if(processDiff < 0) continue;
        process.cpuUsage = (100.0 * processDiff * cpuCount) / totalDiff;
        processes.push_back(process);
    }
}

bool compareCPU(ProcessInfo a, ProcessInfo b) {
    return a.cpuUsage > b.cpuUsage;
}

bool compareMemory(ProcessInfo a, ProcessInfo b) {
    return a.memory > b.memory;
}

bool comparePID(ProcessInfo a, ProcessInfo b) {
    return a.pid < b.pid;
}

void printProcessTable(vector<ProcessInfo> processes, string sortBy, int topN, string nameFilter) {
    if(!nameFilter.empty()) {
        vector<ProcessInfo> filtered;
        for(auto &process : processes) {
            if(process.name.find(nameFilter) != string::npos)
                filtered.push_back(process);
        }
        processes = filtered;
    }

    if(sortBy == "cpu")
        sort(processes.begin(), processes.end(), compareCPU);
    else if(sortBy == "memory")
        sort(processes.begin(), processes.end(), compareMemory);
    else
        sort(processes.begin(), processes.end(), comparePID);

    cout<<"PROCESS MONITOR\n\n";
    cout << left
         << setw(8)  << "PID"
         << setw(24) << "NAME"
         << setw(8)  << "STATE"
         << setw(10) << "PPID"
         << setw(12) << "CPU (%)"
         << setw(14) << "MEMORY"
         << setw(8)  << "THREADS"
         << '\n';
    cout<<"------------------------------------------------------------------------------------\n";

    int count = min(topN, (int)processes.size());
    for(int i = 0; i < count; i++) {
        string name = processes[i].name;
        cout << left
             << setw(8)  << processes[i].pid
             << setw(24) << name
             << setw(8)  << processes[i].state
             << setw(10)  << processes[i].parentPID
             << setw(12) << fixed << setprecision(2) << processes[i].cpuUsage
             << setw(14) << formatBytes(processes[i].memory)
             << setw(8)  << processes[i].threads
             << '\n';
    }
    cout<<"\nProcesses shown: "<<count<<'\n';
    cout<<"Total processes: "<<processes.size()<<'\n';
}

void runProcessCommand(string sortBy, int topN, string nameFilter) {
    vector<ProcessInfo> processes = getProcesses();
    calculateCPUUsage(processes);
    if(processes.empty()) {
        cout<<"Unable to retrieve process information.\n";
        return;
    }
    printProcessTable(processes, sortBy, topN, nameFilter);
}