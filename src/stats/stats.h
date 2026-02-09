#pragma once
#include <string>
#include <vector>

struct PlayerStats {
    std::string name;
    int games = 0;
    int wins = 0;
    int draws = 0;
    int losses = 0;

    long long moves = 0;
    long long totalMoveMs = 0;
    long long fastestMoveMs = -1;
    long long slowestMoveMs = -1;
};

class Stats {
public:
    static bool appendGame(const std::string& filenameCsv,
                           const std::string players[2],
                           int result,
                           long long totalGameMs,
                           long long movesP0,
                           long long movesP1,
                           long long totalMsP0,
                           long long totalMsP1,
                           long long fastestP0,
                           long long fastestP1,
                           long long slowestP0,
                           long long slowestP1);

    static std::vector<PlayerStats> loadAggregated(const std::string& filenameCsv);
    static void printReport(const std::vector<PlayerStats>& stats);
};
