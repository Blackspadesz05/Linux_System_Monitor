#include <iostream>

using namespace std;

void printHelp() {
    cout << "Linux System Monitor\n\n"
         << "Usage:\n"
         << "  sysguard <command>\n\n"
         << "Commands:\n"
         << "  help          Show this help message\n";
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

    cout << "Unknown command: " << command << "\n\n";
    printHelp();

    return 1;
}