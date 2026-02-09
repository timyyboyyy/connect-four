#pragma once

#include <string>
#include <vector>

// Hilfsfunktionen rund um Ein-/Ausgabe
void clearInputLine();
bool isNumber(const std::string& s);
void pressEnterToContinue();
void showCountdown(int seconds);
void clearScreen();
void pressEnterToGoToMain();
void waitEnterAfterCin();
void waitEnterPlain();

std::vector<std::string> listLogFiles(const std::string& folder = "logs/");

