#pragma once

#include <string>
#include <vector>

// Hilfsfunktionen rund um Ein-/Ausgabe
void clearInputLine();
bool isNumber(const std::string& s);
void pressEnterToContinue();

std::vector<std::string> listLogFiles(const std::string& folder = "logs/");
