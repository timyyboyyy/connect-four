#include "board.h"

#include <iostream>
#include <string>

using namespace std;

Board::Board() { reset(); }

void Board::reset() {
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            grid[r][c] = ' ';
}

bool Board::isValidColumn(int col) const {
    return (col >= 0 && col < COLS && grid[0][col] == ' ');
}

int Board::dropDisc(int col, char symbol) {
    if (!isValidColumn(col)) return -1;
    for (int r = ROWS - 1; r >= 0; --r) {
        if (grid[r][col] == ' ') {
            grid[r][col] = symbol;
            return r;
        }
    }
    return -1;
}

bool Board::isFull() const {
    for (int c = 0; c < COLS; ++c)
        if (grid[0][c] == ' ') return false;
    return true;
}

bool Board::checkWin(char s) const {
    // horizontal
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (grid[r][c] == s && grid[r][c + 1] == s && grid[r][c + 2] == s && grid[r][c + 3] == s)
                return true;
        }
    }
    // vertical
    for (int c = 0; c < COLS; ++c) {
        for (int r = 0; r <= ROWS - 4; ++r) {
            if (grid[r][c] == s && grid[r + 1][c] == s && grid[r + 2][c] == s && grid[r + 3][c] == s)
                return true;
        }
    }
    // diag down-right
    for (int r = 0; r <= ROWS - 4; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (grid[r][c] == s && grid[r + 1][c + 1] == s && grid[r + 2][c + 2] == s && grid[r + 3][c + 3] == s)
                return true;
        }
    }
    // diag down-left
    for (int r = 0; r <= ROWS - 4; ++r) {
        for (int c = 3; c < COLS; ++c) {
            if (grid[r][c] == s && grid[r + 1][c - 1] == s && grid[r + 2][c - 2] == s && grid[r + 3][c - 3] == s)
                return true;
        }
    }
    return false;
}

int Board::getDropRow(int col) const {
    if (col < 0 || col >= COLS) return -1;
    if (grid[0][col] != ' ') return -1;

    for (int r = ROWS - 1; r >= 0; --r) {
        if (grid[r][col] == ' ') return r;
    }
    return -1;
}

void Board::printPretty(char p1Symbol, char p2Symbol,
                        int ghostRow, int ghostCol, char ghostSymbol) const {
    auto renderCell = [&](int r, int c) -> std::string {
        if (r == ghostRow && c == ghostCol && ghostSymbol != '\0') {
            if (ghostSymbol == p1Symbol) return "●";
            if (ghostSymbol == p2Symbol) return "○";
            return " ";
        }

        char cell = grid[r][c];
        if (cell == p1Symbol) return "●";
        if (cell == p2Symbol) return "○";
        return " ";
    };

    std::cout << "\n    1   2   3   4   5   6   7\n";
    std::cout << "  ╔═══╦═══╦═══╦═══╦═══╦═══╦═══╗\n";
    for (int r = 0; r < ROWS; ++r) {
        std::cout << "  ║";
        for (int c = 0; c < COLS; ++c) {
            std::cout << " " << renderCell(r, c) << " ║";
        }
        std::cout << "\n";
        if (r != ROWS - 1)
            std::cout << "  ╠═══╬═══╬═══╬═══╬═══╬═══╬═══╣\n";
    }
    std::cout << "  ╚═══╩═══╩═══╩═══╩═══╩═══╩═══╝\n\n";
}
