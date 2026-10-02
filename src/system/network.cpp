#include "network.h"

#include <bits/stdc++.h>

using namespace std;

#define ll long long

string formatBytes(ll val) {
    vector<string> units{"B","KB","MB","GB"};
    ll idx = 0;
    while(idx < units.size() - 1){
        if(val >= 1024){
            val/=1024;
            idx++;
        }
        else break;
    }
    return to_string(val) +" "+ units[idx];
}

void printNetworkTraffic() {
    ifstream file("/proc/net/dev");
    if(!file) {
        cout << "Network: unavailable\n";
        return;
    }
    string line;
    ll totalRX = 0, totalTX = 0;

    while(getline(file, line)) {
        size_t pos = line.find(':');
        if(pos == string::npos) continue;
        string data = line.substr(pos + 1);
        stringstream ss(data);
        ll rx, tx, temp;
        ss >> rx;
        for(int i=0; i<7; i++) ss >> temp;
        ss >> tx;
        totalRX += rx;
        totalTX += tx;
    }
    cout<<"Network RX: "<<formatBytes(totalRX)<<"\n";
    cout<<"Network TX: "<<formatBytes(totalTX)<<"\n";
}