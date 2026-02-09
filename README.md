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

---

## Build & Run

Mit Skripten (falls vorhanden):
- `./build.sh`
- `./run.sh`

Manuell (g++):
- `g++ -std=c++17 -O2 -Wall -Wextra -o connect4 main.cpp`
- `./connect4`

---

## Bedienung

Nach dem Start erscheint ein Menü (z.B. Neues Spiel, Replay, Tests, Beenden).

### Neues Spiel
- Eingabe einer Spalte (typisch **1–7**).
- Der Stein fällt in der gewählten Spalte nach unten.
- Ende bei **4 in einer Reihe** oder **Unentschieden**.

### Replay
- Lädt eine Logdatei und spielt die Züge in Reihenfolge nach.
- Je nach Implementierung: schrittweise (Enter) oder automatisch.

### Tests
- Führt die Test-Suite aus (z.B. Drop, Validierung, Win-Checks).

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
- Logging + Replay
- Test-Suite für Kernfunktionen

---

## Lizenz

Nur für Studien-/Uni-Zwecke (sofern nicht anders angegeben).
