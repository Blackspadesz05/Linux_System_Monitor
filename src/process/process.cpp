#include "process.h"
#include "../util.h"

#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;
#define ll long long

bool isNumber(string str) {
    if(str.empty())
        return false;
    for(char c : str) {
        if(c < '0' || c > '9')
            return false;
    }
    return true;
}

void printProcess(string pid) {
    string path = "/proc/" + pid + "/status";
    ifstream file(path);
    if(!file) return;

    string name = "unknown", state = "unknown", line;
    ll parentPID = 0, memory = 0, threads = 0;
    while(getline(file, line)) {
        size_t pos = line.find(':');
        if(pos == string::npos) continue;

        string key = line.substr(0, pos), value = line.substr(pos + 1);
        while(!value.empty() && (value[0] == ' ' || value[0] == '\t'))
            value.erase(value.begin());
        if(key == "Name") 
            name = value;
        else {
            stringstream ss(value);
            if(key == "State")
                ss >> state;
            else if(key == "PPid")
                ss >> parentPID;
            else if(key == "VmRSS") {
                ll temp; ss >> temp;
                memory = temp * 1024;
            }
            else if(key == "Threads")
                ss >> threads;
        }
    }

    cout << left
         << setw(8)  << pid
         << setw(24) << name
         << setw(8)  << state
         << setw(10) << parentPID
         << setw(14) << formatBytes(memory)
         << setw(8)  << threads
         << '\n';
}

void runProcessCommand() {
    DIR* directory = opendir("/proc");
    if(!directory) {
        cout<<"Unable to access /proc\n";
        return;
    }
    cout<<"PROCESS MONITOR\n\n";
    cout << left
         << setw(8)  << "PID"
         << setw(24) << "NAME"
         << setw(8)  << "STATE"
         << setw(10) << "PPID"
         << setw(14) << "MEMORY"
         << setw(8)  << "THREADS"
         << '\n';
    cout<<"------------------------------------------------------------------------\n";

    struct dirent* entry;
    int processCount = 0;
    while(true){
        entry = readdir(directory);
        if(entry == NULL) break;
        string name = entry->d_name;
        if(!isNumber(name)) continue;
        printProcess(name);
        processCount++;
    }
    closedir(directory);
    cout<<"\nTotal Processes: "<<processCount<<'\n';
}