#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>

using namespace std;

const int ROWS = 6;
const int COLS = 7;

// ---------- Hilfsfunktionen für Eingaben ----------

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void waitForEnter(const string &msg = "Weiter mit Enter...") {
    cout << msg;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ---------- Datenstrukturen ----------

struct Move {
    int playerIndex; // 0 oder 1
    int col;         // 0..6
};

class Board {
private:
    char grid[ROWS][COLS];

public:
    Board() {
        reset();
    }

    void reset() {
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                grid[r][c] = ' ';
            }
        }
    }

    void print() const {
        cout << "\n   1   2   3   4   5   6   7\n";
        cout << "  -----------------------------\n";
        for (int r = 0; r < ROWS; ++r) {
            cout << " |";
            for (int c = 0; c < COLS; ++c) {
                cout << " " << grid[r][c] << " |";
            }
            cout << "\n  -----------------------------\n";
        }
        cout << endl;
    }

    bool isValidColumn(int col) const {
        if (col < 0 || col >= COLS) return false;
        return grid[0][col] == ' ';
    }

    // Disc fallen lassen, Rückgabe: Zeile oder -1
    int dropDisc(int col, char symbol) {
        if (!isValidColumn(col)) return -1;

        for (int r = ROWS - 1; r >= 0; --r) {
            if (grid[r][col] == ' ') {
                grid[r][col] = symbol;
                return r;
            }
        }
        return -1; // sollte eigentlich nie erreicht werden, wenn isValidColumn true
    }

    bool isFull() const {
        for (int c = 0; c < COLS; ++c) {
            if (grid[0][c] == ' ') return false;
        }
        return true;
    }

    char getCell(int r, int c) const {
        return grid[r][c];
    }

    bool checkWin(char symbol) const {
        // Horizontal
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c <= COLS - 4; ++c) {
                if (grid[r][c] == symbol &&
                    grid[r][c + 1] == symbol &&
                    grid[r][c + 2] == symbol &&
                    grid[r][c + 3] == symbol) {
                    return true;
                }
            }
        }

        // Vertikal
        for (int c = 0; c < COLS; ++c) {
            for (int r = 0; r <= ROWS - 4; ++r) {
                if (grid[r][c] == symbol &&
                    grid[r + 1][c] == symbol &&
                    grid[r + 2][c] == symbol &&
                    grid[r + 3][c] == symbol) {
                    return true;
                }
            }
        }

        // Diagonal nach unten rechts
        for (int r = 0; r <= ROWS - 4; ++r) {
            for (int c = 0; c <= COLS - 4; ++c) {
                if (grid[r][c] == symbol &&
                    grid[r + 1][c + 1] == symbol &&
                    grid[r + 2][c + 2] == symbol &&
                    grid[r + 3][c + 3] == symbol) {
                    return true;
                }
            }
        }

        // Diagonal nach unten links
        for (int r = 0; r <= ROWS - 4; ++r) {
            for (int c = 3; c < COLS; ++c) {
                if (grid[r][c] == symbol &&
                    grid[r + 1][c - 1] == symbol &&
                    grid[r + 2][c - 2] == symbol &&
                    grid[r + 3][c - 3] == symbol) {
                    return true;
                }
            }
        }

        return false;
    }
};

// ---------- Logger für Protokollierung / Replay ----------

class GameLogger {
public:
    // Einfaches Textformat:
    // CONNECT4_LOG_V1
    // PLAYER0:<Name>
    // PLAYER1:<Name>
    // SYMBOL0:<Char>
    // SYMBOL1:<Char>
    // RESULT:<-1/0/1>  // -1 = unbekannt, 0 = Unentschieden, 1 = Spieler 0, 2 = Spieler 1
    // MOVE;<playerIndex>;<col>
    static bool save(const string &filename,
                     const vector<Move> &moves,
                     const string players[2],
                     const char symbols[2],
                     int result) {
        ofstream out(filename);
        if (!out) {
            cerr << "Fehler beim Öffnen der Log-Datei zum Schreiben.\n";
            return false;
        }

        out << "CONNECT4_LOG_V1\n";
        out << "PLAYER0:" << players[0] << "\n";
        out << "PLAYER1:" << players[1] << "\n";
        out << "SYMBOL0:" << symbols[0] << "\n";
        out << "SYMBOL1:" << symbols[1] << "\n";
        out << "RESULT:" << result << "\n";

        for (const auto &m : moves) {
            out << "MOVE;" << m.playerIndex << ";" << m.col << "\n";
        }

        return true;
    }

    static bool load(const string &filename,
                     vector<Move> &moves,
                     string players[2],
                     char symbols[2],
                     int &result) {
        ifstream in(filename);
        if (!in) {
            cerr << "Fehler beim Öffnen der Log-Datei zum Lesen.\n";
            return false;
        }

        string line;
        if (!getline(in, line) || line != "CONNECT4_LOG_V1") {
            cerr << "Ungültiges Log-Format.\n";
            return false;
        }

        // Standardwerte
        players[0] = "Spieler 1";
        players[1] = "Spieler 2";
        symbols[0] = 'X';
        symbols[1] = 'O';
        result = -1;

        while (getline(in, line)) {
            if (line.rfind("PLAYER0:", 0) == 0) {
                players[0] = line.substr(8);
            } else if (line.rfind("PLAYER1:", 0) == 0) {
                players[1] = line.substr(8);
            } else if (line.rfind("SYMBOL0:", 0) == 0) {
                if (line.size() > 8) symbols[0] = line[8];
            } else if (line.rfind("SYMBOL1:", 0) == 0) {
                if (line.size() > 8) symbols[1] = line[8];
            } else if (line.rfind("RESULT:", 0) == 0) {
                result = stoi(line.substr(7));
            } else if (line.rfind("MOVE;", 0) == 0) {
                stringstream ss(line);
                string token;
                Move m {-1, -1};

                getline(ss, token, ';'); // "MOVE"
                getline(ss, token, ';');
                m.playerIndex = stoi(token);
                getline(ss, token, ';');
                m.col = stoi(token);

                moves.push_back(m);
            }
        }

        return true;
    }
};

// ---------- Engine für ein Spiel ----------

class GameEngine {
private:
    Board board;
    string players[2];
    char symbols[2] = {'X', 'O'};
    vector<Move> moves;
    int currentPlayer = 0; // 0 oder 1

    int getColumnInput(const string &playerName) {
        int col;
        while (true) {
            cout << playerName << ", wähle eine Spalte (1-7): ";
            if (cin >> col) {
                if (col >= 1 && col <= 7) {
                    return col - 1;
                } else {
                    cout << "Bitte eine Zahl zwischen 1 und 7 eingeben.\n";
                }
            } else {
                cout << "Ungültige Eingabe. Bitte eine Zahl zwischen 1 und 7 eingeben.\n";
                clearInputBuffer();
            }
        }
    }

    string generateLogFileName() {
        time_t now = time(nullptr);
        tm *lt = localtime(&now);
        char buf[64];
        strftime(buf, sizeof(buf), "connect4_%Y%m%d_%H%M%S.log", lt);
        return string(buf);
    }

public:
    GameEngine() = default;

    void startNewGame() {
        board.reset();
        moves.clear();
        currentPlayer = 0;

        clearInputBuffer();
        cout << "Name von Spieler 1 (Symbol " << symbols[0] << "): ";
        getline(cin, players[0]);
        if (players[0].empty()) players[0] = "Spieler 1";

        cout << "Name von Spieler 2 (Symbol " << symbols[1] << "): ";
        getline(cin, players[1]);
        if (players[1].empty()) players[1] = "Spieler 2";

        bool gameOver = false;
        int result = -1; // -1 = noch kein Ergebnis, 0 = Unentschieden, 1/2 = Gewinner-Index+1

        while (!gameOver) {
            board.print();
            string &pName = players[currentPlayer];
            char symbol = symbols[currentPlayer];

            cout << "Am Zug: " << pName << " (" << symbol << ")\n";

            int col = getColumnInput(pName);
            if (!board.isValidColumn(col)) {
                cout << "Diese Spalte ist voll oder ungültig. Bitte eine andere Spalte wählen.\n";
                continue;
            }

            int row = board.dropDisc(col, symbol);
            if (row == -1) {
                cout << "Interner Fehler beim Einwerfen. Versuche es nochmal.\n";
                continue;
            }

            moves.push_back({currentPlayer, col});

            if (board.checkWin(symbol)) {
                board.print();
                cout << "----------------------------------\n";
                cout << "Glückwunsch, " << pName << "! Du hast gewonnen!\n";
                cout << "----------------------------------\n";
                gameOver = true;
                result = currentPlayer + 1;
            } else if (board.isFull()) {
                board.print();
                cout << "----------------------------------\n";
                cout << "Unentschieden! Das Brett ist voll.\n";
                cout << "----------------------------------\n";
                gameOver = true;
                result = 0;
            } else {
                currentPlayer = 1 - currentPlayer;
            }
        }

        // Log speichern
        string filename = generateLogFileName();
        if (GameLogger::save(filename, moves, players, symbols, result)) {
            cout << "Spielprotokoll gespeichert in: " << filename << "\n";
        } else {
            cout << "Spielprotokoll konnte nicht gespeichert werden.\n";
        }
    }

    // Statischer Replay-Modus
    static void replayFromFile(const string &filename) {
        vector<Move> moves;
        string players[2];
        char symbols[2];
        int result;

        if (!GameLogger::load(filename, moves, players, symbols, result)) {
            cout << "Replay konnte nicht geladen werden.\n";
            return;
        }

        cout << "\n=== REPLAY von Datei: " << filename << " ===\n";
        cout << "Spieler 1: " << players[0] << " (" << symbols[0] << ")\n";
        cout << "Spieler 2: " << players[1] << " (" << symbols[1] << ")\n";
        cout << "Ergebnis: ";
        if (result == 0) {
            cout << "Unentschieden\n";
        } else if (result == 1) {
            cout << "Sieg für " << players[0] << "\n";
        } else if (result == 2) {
            cout << "Sieg für " << players[1] << "\n";
        } else {
            cout << "Unbekannt\n";
        }
        cout << "\n";

        Board board;
        board.reset();

        clearInputBuffer();
        int moveNumber = 1;
        for (const auto &m : moves) {
            if (m.playerIndex < 0 || m.playerIndex > 1 || m.col < 0 || m.col >= COLS) {
                cout << "Ungültiger Zug im Log übersprungen.\n";
                continue;
            }
            char symbol = symbols[m.playerIndex];
            int row = board.dropDisc(m.col, symbol);
            if (row == -1) {
                cout << "Ungültiger Zug (Spalte voll) im Replay. Breche ab.\n";
                break;
            }

            cout << "Zug " << moveNumber << ": " << players[m.playerIndex]
                 << " -> Spalte " << (m.col + 1) << "\n";
            board.print();
            ++moveNumber;

            waitForEnter();
        }

        cout << "Replay beendet.\n";
    }
};

// ---------- einfache Testfälle für die Spielmechanik ----------

class TestSuite {
private:
    int passed = 0;
    int failed = 0;

    void assertTrue(bool condition, const string &msg) {
        if (condition) {
            ++passed;
        } else {
            ++failed;
            cout << "[FEHLER] " << msg << "\n";
        }
    }

public:
    void run() {
        cout << "\n=== Starte Test-Suite ===\n";

        // Test 1: Disc fällt nach unten
        {
            Board b;
            int row = b.dropDisc(0, 'X');
            assertTrue(row == ROWS - 1, "Disc sollte in der untersten Reihe landen (Test 1).");
        }

        // Test 2: Spalte füllt sich und wird ungültig
        {
            Board b;
            int col = 3;
            for (int i = 0; i < ROWS; ++i) {
                int r = b.dropDisc(col, 'O');
                assertTrue(r != -1, "Spalte sollte bis zur vollen Höhe gültig sein (Test 2.1).");
            }
            int r = b.dropDisc(col, 'O');
            assertTrue(r == -1, "Drop in volle Spalte muss -1 liefern (Test 2.2).");
        }

        // Test 3: Horizontaler Gewinn
        {
            Board b;
            int r = ROWS - 1;
            b.dropDisc(0, 'X');
            b.dropDisc(1, 'X');
            b.dropDisc(2, 'X');
            b.dropDisc(3, 'X');
            assertTrue(b.checkWin('X'), "Horizontaler Gewinn sollte erkannt werden (Test 3).");
        }

        // Test 4: Vertikaler Gewinn
        {
            Board b;
            int c = 1;
            b.dropDisc(c, 'O');
            b.dropDisc(c, 'O');
            b.dropDisc(c, 'O');
            b.dropDisc(c, 'O');
            assertTrue(b.checkWin('O'), "Vertikaler Gewinn sollte erkannt werden (Test 4).");
        }

        // Test 5: Diagonal nach unten rechts
        {
            Board b;
            // Manuell ein Muster aufbauen
            b.dropDisc(0, 'X');              // (5,0)
            b.dropDisc(1, 'O'); b.dropDisc(1, 'X');      // (5,1),(4,1)
            b.dropDisc(2, 'O'); b.dropDisc(2, 'O'); b.dropDisc(2, 'X'); // (5,2),(4,2),(3,2)
            b.dropDisc(3, 'O'); b.dropDisc(3, 'O'); b.dropDisc(3, 'O'); b.dropDisc(3, 'X'); // (5,3),(4,3),(3,3),(2,3)

            assertTrue(b.checkWin('X'), "Diagonal (\\) Gewinn sollte erkannt werden (Test 5).");
        }

        // Test 6: Diagonal nach unten links
        {
            Board b;
            b.dropDisc(3, 'X');              // (5,3)
            b.dropDisc(2, 'O'); b.dropDisc(2, 'X');      // (5,2),(4,2)
            b.dropDisc(1, 'O'); b.dropDisc(1, 'O'); b.dropDisc(1, 'X'); // (5,1),(4,1),(3,1)
            b.dropDisc(0, 'O'); b.dropDisc(0, 'O'); b.dropDisc(0, 'O'); b.dropDisc(0, 'X'); // (5,0),(4,0),(3,0),(2,0)

            assertTrue(b.checkWin('X'), "Diagonal (/) Gewinn sollte erkannt werden (Test 6).");
        }

        // Test 7: Volles Brett
        {
            Board b;
            char symbols[2] = {'X', 'O'};
            int idx = 0;
            for (int c = 0; c < COLS; ++c) {
                for (int r = 0; r < ROWS; ++r) {
                    b.dropDisc(c, symbols[idx]);
                    idx = 1 - idx;
                }
            }
            assertTrue(b.isFull(), "Brett sollte als voll erkannt werden (Test 7).");
        }

        cout << "Tests bestanden: " << passed << "\n";
        cout << "Tests fehlgeschlagen: " << failed << "\n";
        cout << "=== Test-Suite beendet ===\n\n";
    }
};

// ---------- main: Menü & Steuerung ----------

int main() {
    GameEngine engine;
    TestSuite tests;

    while (true) {
        cout << "==== 4 GEWINNT - SPIELENGINE ====\n";
        cout << "1) Neues Spiel starten\n";
        cout << "2) Spiel aus Logdatei wiederholen\n";
        cout << "3) Tests ausführen\n";
        cout << "4) Beenden\n";
        cout << "Auswahl: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Bitte eine gültige Zahl eingeben.\n";
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            engine.startNewGame();
        } else if (choice == 2) {
            cout << "Dateinamen der Logdatei eingeben: ";
            string filename;
            cin >> filename;
            GameEngine::replayFromFile(filename);
        } else if (choice == 3) {
            tests.run();
        } else if (choice == 4) {
            cout << "Programm wird beendet.\n";
            break;
        } else {
            cout << "Ungültige Auswahl.\n";
        }
    }

    return 0;
}
