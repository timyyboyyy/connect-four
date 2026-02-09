#include "stats.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

static void updateMinMax(long long v, long long& mn, long long& mx) {
    if (v < 0) return;
    if (mn < 0 || v < mn) mn = v;
    if (mx < 0 || v > mx) mx = v;
}

bool Stats::appendGame(const string& filenameCsv,
                       const string players[2],
                       int result,
                       long long totalGameMs,
                       long long movesP0,
                       long long movesP1,
                       long long totalMsP0,
                       long long totalMsP1,
                       long long fastestP0,
                       long long fastestP1,
                       long long slowestP0,
                       long long slowestP1) {
    bool writeHeader = false;
    { ifstream test(filenameCsv); if (!test.good()) writeHeader = true; }

    ofstream out(filenameCsv, ios::app);
    if (!out) return false;

    if (writeHeader) {
        out << "player0,player1,result,totalGameMs,"
               "moves0,moves1,totalMs0,totalMs1,fastest0,fastest1,slowest0,slowest1\n";
    }

    out << players[0] << ","
        << players[1] << ","
        << result << ","
        << totalGameMs << ","
        << movesP0 << ","
        << movesP1 << ","
        << totalMsP0 << ","
        << totalMsP1 << ","
        << fastestP0 << ","
        << fastestP1 << ","
        << slowestP0 << ","
        << slowestP1 << "\n";

    return true;
}

vector<PlayerStats> Stats::loadAggregated(const string& filenameCsv) {
    ifstream in(filenameCsv);
    if (!in) return {};

    string header;
    getline(in, header);

    unordered_map<string, PlayerStats> mapStats;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string p0, p1, tok;
        int result = -1;
        long long totalGameMs=0, moves0=0, moves1=0, totalMs0=0, totalMs1=0;
        long long fastest0=-1, fastest1=-1, slowest0=-1, slowest1=-1;

        getline(ss, p0, ',');
        getline(ss, p1, ',');
        getline(ss, tok, ','); result = stoi(tok);
        getline(ss, tok, ','); totalGameMs = stoll(tok);
        getline(ss, tok, ','); moves0 = stoll(tok);
        getline(ss, tok, ','); moves1 = stoll(tok);
        getline(ss, tok, ','); totalMs0 = stoll(tok);
        getline(ss, tok, ','); totalMs1 = stoll(tok);
        getline(ss, tok, ','); fastest0 = stoll(tok);
        getline(ss, tok, ','); fastest1 = stoll(tok);
        getline(ss, tok, ','); slowest0 = stoll(tok);
        getline(ss, tok, ','); slowest1 = stoll(tok);

        auto& s0 = mapStats[p0];
        s0.name = p0; s0.games++;
        s0.moves += moves0; s0.totalMoveMs += totalMs0;
        updateMinMax(fastest0, s0.fastestMoveMs, s0.slowestMoveMs);
        updateMinMax(slowest0, s0.fastestMoveMs, s0.slowestMoveMs);

        auto& s1 = mapStats[p1];
        s1.name = p1; s1.games++;
        s1.moves += moves1; s1.totalMoveMs += totalMs1;
        updateMinMax(fastest1, s1.fastestMoveMs, s1.slowestMoveMs);
        updateMinMax(slowest1, s1.fastestMoveMs, s1.slowestMoveMs);

        if (result == 0) { s0.draws++; s1.draws++; }
        else if (result == 1) { s0.wins++; s1.losses++; }
        else if (result == 2) { s1.wins++; s0.losses++; }
    }

    vector<PlayerStats> out;
    out.reserve(mapStats.size());
    for (auto& kv : mapStats) out.push_back(kv.second);

    sort(out.begin(), out.end(), [](const PlayerStats& a, const PlayerStats& b){
        return a.wins > b.wins;
    });

    return out;
}

void Stats::printReport(const vector<PlayerStats>& stats) {
    if (stats.empty()) {
        cout << "Keine Statistikdaten vorhanden (logs/stats.csv).\n\n";
        return;
    }

    cout << "\n=== Spieler-Statistiken ===\n";
    cout << "Name | Spiele | W | U | L | Zuege | Ø ms/Zug | Schnellster | Langsamster\n";
    cout << "-----------------------------------------------------------------------\n";

    for (const auto& s : stats) {
        long long avg = (s.moves > 0) ? (s.totalMoveMs / s.moves) : 0;
        cout << s.name << " | "
             << s.games << " | "
             << s.wins << " | "
             << s.draws << " | "
             << s.losses << " | "
             << s.moves << " | "
             << avg << " | "
             << s.fastestMoveMs << " | "
             << s.slowestMoveMs << "\n";
    }
    cout << "\n";
}
