#include <iostream>
#include <string>
#include <vector>

#include "gameengine/gameengine.h"
#include "testsuite/testsuite.h"
#include "util/util.h"
#include "stats/stats.h"
#include "mainmenu/mainmenu.h"

using namespace std;

int main() {
    GameEngine engine;
    TestSuite tests;

    while (true) {
        clearScreen();
        int choice = MainMenu::show();
        if (choice == -1) {
            cout << "Ungültige Eingabe.\n";
            clearInputLine();
            continue;
        }
        
        if (choice == 1) {
            //clearScreen();
            engine.startNewGame();
            waitEnterPlain();
        } else if (choice == 2) {
            clearScreen();
            auto logs = listLogFiles("logs");

            if (logs.empty()) {
                cout << "Keine Logdateien im Ordner 'logs/' gefunden.\n\n";
                waitEnterPlain();
                continue;
            }

            cout << "\nVerfügbare Logdateien:\n";
            for (size_t i = 0; i < logs.size(); ++i) {
                // schöner anzeigen ohne "logs/"
                std::string display = logs[i];
                if (display.rfind("logs/", 0) == 0) display = display.substr(5);
                cout << "  " << (i + 1) << ") " << display << "\n";
            }

            cout << "\nNummer wählen (1-" << logs.size() << ") oder 0 zum Abbrechen: ";
            int idx;
            if (!(cin >> idx)) {
                cout << "Ungültige Eingabe.\n";
                clearInputLine();
                continue;
            }

            if (idx == 0) {
                cout << "Abgebrochen.\n\n";
                continue;
            }
            if (idx < 1 || static_cast<size_t>(idx) > logs.size()) {
                cout << "Bitte eine gültige Nummer wählen.\n\n";
                continue;
            }

            GameEngine::replayFromFile(logs[idx - 1]);
            waitEnterPlain();
        } else if (choice == 3) {
            clearScreen();
            tests.run();
            waitEnterPlain();
        } else if (choice == 4) {
            clearScreen();
            auto stats = Stats::loadAggregated("logs/stats.csv");
            Stats::printReport(stats);
            waitEnterPlain();
        } else if (choice == 5 || choice == 0) {
            cout << "Beenden.\n";
            break;
        } else {
            cout << "Bitte 1-5 wählen.\n";
        }
    }

    return 0;
}
