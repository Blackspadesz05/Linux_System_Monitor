#include "util.h"

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