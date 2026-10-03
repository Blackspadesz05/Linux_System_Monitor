#include "network.h"
#include "../util.h"

#include <bits/stdc++.h>

using namespace std;

#define ll long long

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
        string interface = line.substr(0, pos);
        while(!interface.empty() && interface[0] == ' ')
            interface.erase(interface.begin());

        string data = line.substr(pos + 1);
        stringstream ss(data);
        ll rx, tx, temp;
        ss >> rx;
        for(int i=0; i<7; i++) ss >> temp;
        ss >> tx;
        totalRX += rx;
        totalTX += tx;
        cout<<"Network "<<interface<<" RX: "<<formatBytes(rx)<<" TX: "<<formatBytes(tx)<<"\n";
    }
    cout<<"Network Total RX: "<<formatBytes(totalRX)<<"\n";
    cout<<"Network Total TX: "<<formatBytes(totalTX)<<"\n";
}