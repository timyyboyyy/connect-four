# Vier Gewinnt – Spiele-Engine

Dieses Projekt implementiert das klassische Brettspiel **„Vier Gewinnt“** als Konsolenanwendung in C++ (C++17). Enthalten sind ein Spieler-gegen-Spieler-Modus (PvP), korrekte Regelprüfung, Protokollierung (Logdatei), ein Replay-Modus sowie eine TestSuite zur Verifikation der Spielmechanik.

## Umgebung / Voraussetzungen

Plattform ist Linux. Als Programmiersprache wird C++ im Standard C++17 verwendet und mit `g++` (GNU Compiler Collection) kompiliert. Es werden keine externen Bibliotheken benötigt, sondern ausschließlich die C++-Standardbibliothek verwendet. Sofern `<filesystem>` vom System/Compiler unterstützt wird, werden Ordner wie `logs/` automatisch erstellt.

## Projektstruktur

```plaintext
.
├─ src/
│  └─ main.cpp  
├─ logs/
├─ data/
├─ build.sh  
├─ run.sh  
└─ README.md  
```

## Kompilieren

Empfohlen ist das Build-Skript zu verwenden:

```bash
chmod +x build.sh
./build.sh
```

## Starten

Empfohlen ist das Run-Skript zu verwenden:

```bash
chmod +x run.sh
./run.sh
```

