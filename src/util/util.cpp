#include "util.h"

#include <cctype>
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
  #define HAS_FILESYSTEM 1
#else
  #define HAS_FILESYSTEM 0
#endif

using namespace std;

void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char ch : s) {
        if (!isdigit(static_cast<unsigned char>(ch))) return false;
    }
    return true;
}

void pressEnterToContinue() {
    cout << "Weiter mit Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showCountdown(int seconds) {
    using namespace std;
    for (int i = seconds; i > 0; --i) {
        cout << "\rZeitlimit: " << i << "s " << flush;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\rZeitlimit: 0s \n";
}

vector<string> listLogFiles(const string& folder) {
    vector<string> files;

#if HAS_FILESYSTEM
    try {
        if (!fs::exists(folder) || !fs::is_directory(folder)) return files;

        for (const auto& entry : fs::directory_iterator(folder)) {
            if (!entry.is_regular_file()) continue;

            auto p = entry.path();
            if (p.extension() == ".log") {
                files.push_back(p.string());
            }
        }

        sort(files.rbegin(), files.rend()); // neueste zuerst
    } catch (...) {
    }
#endif

    return files;
}

void clearScreen() {
    cout << "\033[2J\033[H\033[3J" << flush;
}

void pressEnterToGoToMain() {
    cout << "\nZurück zum Hauptmenü mit Enter...";
    cout.flush();

    string dummy;
    getline(cin, dummy);
}

void waitEnterAfterCin() {
    cout << "\nZurück zum Hauptmenü mit Enter...";
    cout.flush();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string dummy;
    getline(cin, dummy);
}

void waitEnterPlain() {
    cout << "\nZurück zum Hauptmenü mit Enter...";
    cout.flush();

    string dummy;
    getline(cin, dummy);
}