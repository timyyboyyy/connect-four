# Connect Four (4 Gewinnt) – C++ Konsolenprojekt

Dieses Projekt implementiert **4 Gewinnt (Connect Four)** als Konsolenanwendung in C++.  
Enthalten sind **Menü**, **2-Spieler-Spiel** (7×6), **Zugvalidierung**, **Gewinn-/Unentschieden-Erkennung**, **Logging** sowie **Replay** aus einer Logdatei. Zusätzlich gibt es eine **Test-Suite**.

---

## Features

- 7 Spalten × 6 Reihen Spielfeld
- Zwei Spieler (X / O), abwechselnde Züge
- Eingabevalidierung (ungültige Spalte, volle Spalte, falsches Format)
- Gewinnprüfung: horizontal, vertikal, diagonal
- Unentschieden-Erkennung (Board voll)
- Logdatei wird beim Spielen geschrieben
- Replay-Modus: Spiel kann aus Logdatei wiedergegeben werden
- Test-Suite zur Prüfung der Kernlogik
- Statistische Auswertungen der Spieler

---

---

## Voraussetzungen

- Linux / macOS / WSL (getestet unter WSL)
- **g++ mit C++17** (oder kompatibler Compiler)
- `bash`


### Installation g++ mit C++17 auf Debian/Ubuntu (falls nicht vorhanden)

```bash
sudo apt update && sudo apt upgrade
sudo apt install -y build-essential g++ bash

g++ --version
bash --version
```

### Installation g++ mit C++17 auf Fedora (falls nicht vorhanden)

```bash
sudo dnf install -y gcc-c++ bash

g++ --version
bash --version
```

---

## Build & Run

Mit Skripten (empfohlen):
```bash
chmod +x build.sh run.sh
./build.sh
./run.sh
```

Manuell (g++):
```bash
g++ -std=c++17 -O2 -Wall -Wextra -o connect4 main.cpp
./connect4
```

---

## Bedienung

Nach dem Start erscheint ein Menü, durch welches mit den Pfeiltasten navigiert wird (z.B. Neues Spiel, Replay, Tests, Statistiken, Beenden).

### Neues Spiel
- Eingabe einer Spalte (typisch **1–7**).
- Der Stein fällt in der gewählten Spalte nach unten.
- Ende bei **4 in einer Reihe** oder **Unentschieden**.

### Replay
- Lädt eine Logdatei und spielt die Züge in Reihenfolge nach.

### Tests
- Führt die Test-Suite aus (z.B. Drop, Validierung, Win-Checks).

### Statistiken
- Zeigt für jeden aufgezeichneten Spieler dessen Statistiken an.

---

## Logdatei / Replay-Format

Es existieren zwei mögliche Header-Varianten:
- `CONNECT4_LOG_V1`
- `CONNECT4_LOG_V2`

Bedeutung pro Zugzeile: `Spieler Spalte [moveTimeMs]`

- `Spieler`: `X` oder `O`
- `Spalte`: Zielspalte (0–6 oder 1–7, je nach Implementierung)
- `moveTimeMs` (optional, V2): Zugzeit in Millisekunden (falls erfasst)

Hinweis: Wenn `moveTimeMs` fehlt, sollte die Zeile trotzdem verarbeitet werden (Fallback wie V1).

---

## Hinweise / Randfälle

- Ungültige Eingaben (nicht-numerisch, außerhalb des Bereichs) werden abgefangen.
- Volle Spalten sind nicht spielbar und werden abgelehnt.
- Bei Replay: beschädigte oder inkompatible Logdateien sollten eine verständliche Fehlermeldung ausgeben.

---

## Inhalt (Kurz)

- Spielbrett (7×6), Drop-Mechanik, Win/Draw
- Menüführung
- Logging + Replay + Analyse
- Test-Suite für Kernfunktionen

---

## Lizenz

Nur für Studien-/Uni-Zwecke.
