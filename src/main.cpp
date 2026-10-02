#include <bits/stdc++.h>
#include "system/system.h"

using namespace std;

void printHelp() {
    cout << "Linux System Monitor\n\n"
         << "Usage:\n"
         << "  sysguard <command>\n\n"
         << "Commands:\n"
         << "  system               Show system information\n"
         << "  system --watch       Continuously monitor the system\n"
         << "  system --watch --refresh N \n"
         << "                       Refresh every N seconds\n"
         << "  help                 Show this help message\n";
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
        bool watch = false;
        int refresh = 10;
        for(int i=2; i<argc; i++) {
            string option = argv[i];
            if(option == "--watch") watch = true;
            else if(option == "--refresh") {
                if(i + 1 >= argc) {
                    cout<<"Missing value for --refresh\n";
                    return 1;
                }
                refresh = stoi(argv[++i]);
                if(refresh <= 0) {
                    cout<<"Refresh interval must be positive\n";
                    return 1;
                }
            }
            else {
                cout<<"Unknown option: "<<option<<"\n";
                return 1;
            }
        }
        if(!watch && refresh != 10) {
            cout<<"--refresh can only be used with --watch\n";
            return 1;
        }
        runSystemCommand(watch, refresh);
        return 0;
    }

    cout << "Unknown command: " << command << "\n\n";
    printHelp();

    return 1;
}