#include "network.h"
#include "../util.h"

#include <bits/stdc++.h>
#include <chrono>
#include <thread>

using namespace std;

#define ll long long

map<string, NetworkInterfaceStats> getNetworkStats() {
    map<string, NetworkInterfaceStats> stats;
    ifstream file("/proc/net/dev");
    if(!file)
        return stats;

    string line;
    while(getline(file, line)) {
        size_t pos = line.find(':');
        if(pos == string::npos) continue;
        string interface = line.substr(0, pos);
        while(!interface.empty() && interface[0] == ' ')
            interface.erase(interface.begin());
        while(!interface.empty() && interface.back() == ' ')
            interface.pop_back();

        string data = line.substr(pos + 1);
        stringstream ss(data);
        NetworkInterfaceStats current;
        ll temp;
        if(!(ss >> current.rxBytes >> current.rxPackets >> current.rxErrors >> current.rxDropped))
            continue;
        for(int i = 0; i < 4; i++)
            ss >> temp;
        if(!(ss >> current.txBytes >> current.txPackets >> current.txErrors >> current.txDropped))
            continue;
        stats[interface] = current;
    }
    return stats;
}

NetworkValues printNetworkTraffic(int sampleTime, const string &filter) {
    NetworkValues result;
    map<string, NetworkInterfaceStats> start = getNetworkStats();

    if(start.empty()) {
        cout<<"Network: unavailable\n";
        return result;
    }
    cout<<"[Calculating Network Traffic...]\n";

    this_thread::sleep_for(chrono::seconds(sampleTime));
    map<string, NetworkInterfaceStats> end = getNetworkStats();
    if(end.empty()) {
        cout<<"Network: unavailable\n";
        return result;
    }

    cout<<"\nNETWORK MONITOR\n\n";
    ll totalRx = 0, totalTx = 0, totalRxPackets = 0, totalTxPackets = 0;
    bool found = false;

    for(auto &entry : end) {
        string interface = entry.first;
        if(!filter.empty() && interface != filter)
            continue;
        if(start.find(interface) == start.end())
            continue;
        NetworkInterfaceStats oldStats = start[interface];
        NetworkInterfaceStats newStats = entry.second;

        ll rxBytes = newStats.rxBytes - oldStats.rxBytes;
        ll txBytes = newStats.txBytes - oldStats.txBytes;
        ll rxPackets = newStats.rxPackets - oldStats.rxPackets;
        ll txPackets = newStats.txPackets - oldStats.txPackets;
        ll rxErrors = newStats.rxErrors - oldStats.rxErrors;
        ll txErrors = newStats.txErrors - oldStats.txErrors;
        ll rxDropped = newStats.rxDropped - oldStats.rxDropped;
        ll txDropped = newStats.txDropped - oldStats.txDropped;
        if(rxBytes < 0) rxBytes = 0;
        if(txBytes < 0) txBytes = 0;
        if(rxPackets < 0) rxPackets = 0;
        if(txPackets < 0) txPackets = 0;

        cout<<"Interface: "<<interface<<'\n';
        cout<<"  RX: "<<formatBytes(rxBytes / sampleTime)<<"/s ("<<rxPackets / sampleTime<<" packets/s)\n";
        cout<<"  TX: "<<formatBytes(txBytes / sampleTime)<<"/s ("<<txPackets / sampleTime<<" packets/s)\n";
        cout<<"  Errors: RX="<<rxErrors<<" TX="<<txErrors<<'\n';
        cout<<"  Drops:  RX="<<rxDropped<<" TX="<<txDropped<<"\n\n";

        totalRx += rxBytes / sampleTime;
        totalTx += txBytes / sampleTime;
        totalRxPackets += rxPackets / sampleTime;
        totalTxPackets += txPackets / sampleTime;
        found = true;
    }

    if(!found) {
        if(filter.empty())
            cout << "No network interfaces found.\n";
        else
            cout << "Interface not found: " << filter << '\n';
        return result;
    }
    cout<<"TOTAL NETWORK TRAFFIC\n";
    cout<<"RX: "<<formatBytes(totalRx)<<"/s ("<<totalRxPackets<<" packets/s)\n";
    cout<<"TX: "<<formatBytes(totalTx)<<"/s ("<<totalTxPackets<<" packets/s)\n";

    result.rxBytesPerSec = totalRx;
    result.txBytesPerSec = totalTx;
    return result;
}

void runNetworkCommand(bool watch, int refresh, const string &filter) {
    if(!watch) {
        printNetworkTraffic(1, filter);
        return;
    }
    while(true) {
        clearScreen();
        printNetworkTraffic(1, filter);
        cout << "\nPress Ctrl+C to exit watch mode.\n";
        if(refresh > 1)
            this_thread::sleep_for(chrono::seconds(refresh-1));
    }
}