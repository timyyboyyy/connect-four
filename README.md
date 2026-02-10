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

## Voraussetzungen

- Linux / macOS / WSL (getestet unter WSL)
- C++17-Compiler (**g++** oder kompatibel, z. B. **clang++** auf macOS)
- `bash`


## Installation 

### Debian/Ubunut (Linux/WSL)

```bash
sudo apt update && sudo apt upgrade
sudo apt install -y build-essential g++ bash

g++ --version
bash --version
```

### Fedora (Linux)

```bash
sudo dnf install -y gcc-c++ bash

g++ --version
bash --version
```

### macOS

Auf macOS ist standartmäßig clang++ über die Xcode Command Line Tools verfügbar.

```bash
xcode-select --install

clang++ --version
bash --version
```
(Optional, falls lieber mit GNU g++ gearbeitet wird)
```bash
brew install gcc

g++-14 --version
```

---

## Build & Run

Mit Skripten (empfohlen):
```bash
chmod +x build_linux.sh build_mac.sh run.sh

#Linux
./build_linux.sh

#macOS
./build_mac.sh

#Ausführen
./run.sh
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

## License

Copyright (c) 2026 Tim

Licensed under the MIT License – see the [LICENSE](LICENSE) file for details.