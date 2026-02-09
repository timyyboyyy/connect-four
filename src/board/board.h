#pragma once

#include "constants.h"

// Spielfeld / Regeln rund um das Setzen und Gewinnen
class Board {
private:
    char grid[ROWS][COLS];

public:
    Board();

    void reset();
    bool isValidColumn(int col) const;
    int dropDisc(int col, char symbol);
    bool isFull() const;
    bool checkWin(char s) const;

    int getDropRow(int col) const;

    // "Bildschirmmaske": hübsches Brett (Box Drawing)
    void printPretty(char p1Symbol, char p2Symbol,
                     int ghostRow = -1, int ghostCol = -1, char ghostSymbol = '\0') const;

};
