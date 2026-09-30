#include <bits/stdc++.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>

using namespace std;
#define ll long long

void printHelp() {
    cout << "Linux System Monitor\n\n"
         << "Usage:\n"
         << "  sysguard <command>\n\n"
         << "Commands:\n"
         << "  system        Show system information\n"
         << "  help          Show this help message\n";
}

void printSystemInfo() {
    utsname info;
    if(uname(&info) != 0) {
        cout<<"Unable to retrieve system information.\n";
        return;
    }
    char hostname[64];
    if(gethostname(hostname, sizeof(hostname)) != 0) cout<<"Hostname: unavailable\n";
    else cout<<"Hostname: "<<hostname<<'\n';
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

void printDisk() {
    struct statvfs stats;
    if(statvfs("/", &stats) != 0) {
        cout<<"Disk Usage: unavailable\n";
        return;
    }
    ll total = (ll)(stats.f_blocks) * stats.f_frsize;
    ll available = (ll)(stats.f_bavail) * stats.f_frsize;
    ll used = total - available;
    double usage = 100.0 * (double)used/total;
    cout<<"Disk Usage: "<<usage<<"%\n";
}

void runSystemCommand() {
    cout<<"SYSTEM: \n";
    printSystemInfo();
    printUptime();
    printLoadAverage();

    cout<<"\nRESOURCES: \n";
    printMemory();
    printDisk();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    string command = argv[1];
    if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
        return 0;
    }
    if (command == "system") {
        runSystemCommand();
        return 0;
    }

    cout << "Unknown command: " << command << "\n\n";
    printHelp();

    return 1;
}