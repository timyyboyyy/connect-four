#include <iostream>
#include <string>
#include <vector>

#include "gameengine/gameengine.h"
#include "testsuite/testsuite.h"
#include "util/util.h"
#include "stats/stats.h"

using namespace std;

int main() {
    GameEngine engine;
    TestSuite tests;

    while (true) {
        cout << "==== 4 GEWINNT - SPIELENGINE ====\n";
        cout << "1) Neues Spiel starten\n";
        cout << "2) Spiel aus Logdatei wiederholen\n";
        cout << "3) Tests ausführen\n";
        cout << "4) Statistiken anzeigen\n";
        cout << "5) Beenden\n";
        cout << "Auswahl: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Ungültige Eingabe.\n";
            clearInputLine();
            continue;
        }

        if (choice == 1) {
            engine.startNewGame();
        } else if (choice == 2) {
            auto logs = listLogFiles("logs");

            if (logs.empty()) {
                cout << "Keine Logdateien im Ordner 'logs/' gefunden.\n\n";
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

        } else if (choice == 3) {
            tests.run();
        } else if (choice == 4) {
            auto stats = Stats::loadAggregated("logs/stats.csv");
            Stats::printReport(stats);
        } else if (choice == 5) {
            cout << "Beenden.\n";
            break;
        } else {
            cout << "Bitte 1-4 wählen.\n";
        }
    }

    return 0;
}
