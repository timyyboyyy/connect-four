#include "gameengine.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "board/board.h"
#include "gamelogger/gamelogger.h"
#include "move.h"
#include "util/util.h"
#include "stats/stats.h"

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
  #define HAS_FILESYSTEM 1
#else
  #define HAS_FILESYSTEM 0
#endif

using namespace std;

namespace {
    string makeLogFileName() {
        time_t now = time(nullptr);
        tm *lt = localtime(&now);
        char buf[64];
        strftime(buf, sizeof(buf), "connect4_%Y%m%d_%H%M%S.log", lt);

    #if HAS_FILESYSTEM
        try { fs::create_directories("logs"); } catch (...) {}
        return string("logs/") + buf;
    #else
        return string(buf);
    #endif
    }

    // Rückgabe:
    // -1 => Abbruch
    //  0..6 => gültige Spalte
    int readColumnOrAbort(const string &playerName) {
        while (true) {
            cout << "Du hast " << (TURN_LIMIT_MS/1000) << " Sekunden.\n";
            cout << playerName << " wählt Spalte (1-7) | 0/q = Abbruch: ";
            string input;
            cin >> input;

            if (input == "0" || input == "q" || input == "Q") return -1;

            if (!isNumber(input)) {
                cout << "Ungültig. Bitte 1-7 oder 0/q.\n";
                continue;
            }

            int col = stoi(input);
            if (col < 1 || col > 7) {
                cout << "Bitte nur 1 bis 7 (oder 0/q).\n";
                continue;
            }
            return col - 1;
        }
    }
}

void GameEngine::startNewGame() {
    Board board;
    string players[2];
    char symbols[2] = {'X', 'O'};
    vector<Move> moves;
    int currentPlayer = 0;

    board.reset();
    moves.clear();
    currentPlayer = 0;

    clearInputLine(); // falls vorher cin >> genutzt wurde
    cout << "\n=== Neues Spiel ===\n";
    cout << "Name Spieler 1 (Symbol X / Anzeige ●): ";
    getline(cin, players[0]);
    if (players[0].empty()) players[0] = "Spieler 1";

    cout << "Name Spieler 2 (Symbol O / Anzeige ○): ";
    getline(cin, players[1]);
    if (players[1].empty()) players[1] = "Spieler 2";

    bool gameOver = false;
    int result = -1; // 0 draw, 1 p0 win, 2 p1 win, -1 none

    while (!gameOver) {
        board.printPretty(symbols[0], symbols[1]);

        const string &pName = players[currentPlayer];
        char sym = symbols[currentPlayer];

        cout << "Am Zug: " << pName << " (" << (currentPlayer == 0 ? "●" : "○") << ")\n";

        auto tStart = chrono::steady_clock::now();
        int col = readColumnOrAbort(pName);
        auto tEnd = chrono::steady_clock::now();
        long long ms = chrono::duration_cast<chrono::milliseconds>(tEnd - tStart).count();

        if (ms > TURN_LIMIT_MS) {
            cout << "Zeitlimit überschritten (" << ms << " ms > " << TURN_LIMIT_MS << " ms).\n";
            cout << "Sieg für " << players[1 - currentPlayer] << "!\n";
            result = (currentPlayer == 0) ? 2 : 1;
            gameOver = true;
            break;
        }

        if (col == -1) {
            cout << "Spiel wurde abgebrochen. Kein Log wird gespeichert.\n\n";
            clearInputLine();
            return; // zurück ins Menü
        }

        if (!board.isValidColumn(col)) {
            cout << "Diese Spalte ist voll. Bitte eine andere Spalte wählen.\n";
            continue;
        }

        if (board.dropDisc(col, sym) == -1) {
            cout << "Interner Fehler beim Setzen. Bitte erneut.\n";
            continue;
        }

        moves.push_back({currentPlayer, col, ms});

        if (board.checkWin(sym)) {
            board.printPretty(symbols[0], symbols[1]);
            cout << "Sieg für " << pName << "!\n";
            result = currentPlayer + 1;
            gameOver = true;
        } else if (board.isFull()) {
            board.printPretty(symbols[0], symbols[1]);
            cout << "Unentschieden (Brett voll).\n";
            result = 0;
            gameOver = true;
        } else {
            currentPlayer = 1 - currentPlayer;
        }
    }

    // Statistiken sammeln
    long long totalGameMs = 0;
    long long movesP0 = 0, movesP1 = 0;
    long long totalMsP0 = 0, totalMsP1 = 0;
    long long fastestP0 = -1, fastestP1 = -1;
    long long slowestP0 = -1, slowestP1 = -1;

    auto upd = [](long long v, long long& mn, long long& mx){
        if (mn < 0 || v < mn) mn = v;
        if (mx < 0 || v > mx) mx = v;
    };

    for (const auto& m : moves) {
        totalGameMs += m.moveTimeMs;
        if (m.playerIndex == 0) {
            movesP0++; totalMsP0 += m.moveTimeMs;
            upd(m.moveTimeMs, fastestP0, slowestP0);
        } else {
            movesP1++; totalMsP1 += m.moveTimeMs;
            upd(m.moveTimeMs, fastestP1, slowestP1);
        }
    }

    // Spiel in stats.csv schreiben
    #if HAS_FILESYSTEM
    try { fs::create_directories("logs"); } catch (...) {}
    #endif

    Stats::appendGame("logs/stats.csv", players, result, totalGameMs,
                    movesP0, movesP1, totalMsP0, totalMsP1,
                    fastestP0, fastestP1, slowestP0, slowestP1);


    // speichern
    string fn = makeLogFileName();
    if (GameLogger::save(fn, moves, players, symbols, result)) {
        cout << "Log gespeichert: " << fn << "\n\n";
    } else {
        cout << "Fehler: Log konnte nicht gespeichert werden.\n\n";
    }
}

void GameEngine::replayFromFile(const string &filename) {
    vector<Move> moves;
    string players[2];
    char symbols[2];
    int result = -1;

    if (!GameLogger::load(filename, moves, players, symbols, result)) {
        cout << "Replay fehlgeschlagen: Datei nicht lesbar oder falsches Format.\n\n";
        return;
    }

    cout << "\n=== Replay: " << filename << " ===\n";
    cout << "Spieler 1: " << players[0] << " (●)\n";
    cout << "Spieler 2: " << players[1] << " (○)\n";
    cout << "Hinweis: Enter = nächster Zug, q = abbrechen\n\n";

    clearInputLine();

    Board board;
    int moveNr = 1;

    for (const auto &m : moves) {
        cout << "Zug " << moveNr << ": " << players[m.playerIndex]
             << " -> Spalte " << (m.col + 1);
        if (m.moveTimeMs > 0) cout << " (" << m.moveTimeMs << " ms)";
        cout << "\n";

        if (board.dropDisc(m.col, symbols[m.playerIndex]) == -1) {
            cout << "Replay abgebrochen: Ungültiger Zug im Log (Spalte voll).\n";
            break;
        }

        board.printPretty(symbols[0], symbols[1]);

        // Schrittweise Wiedergabe
        string line;
        getline(cin, line);
        if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
            cout << "Replay abgebrochen.\n\n";
            return;
        }

        ++moveNr;
    }

    cout << "Replay beendet. Ergebnis: ";
    if (result == 0) cout << "Unentschieden\n\n";
    else if (result == 1) cout << "Sieg " << players[0] << "\n\n";
    else if (result == 2) cout << "Sieg " << players[1] << "\n\n";
    else cout << "Unbekannt\n\n";
}
