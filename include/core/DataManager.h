// DataManager.h
// Handles all file I/O for the application. Statistics and match
// history are stored as simple, human-readable pipe-delimited text
// files under the data/ directory -- no external JSON dependency is
// required, which keeps the build self-contained.
#pragma once

#include <string>

#include "core/StatisticsManager.h"
#include "core/GameHistory.h"

namespace rps {

class DataManager {
public:
    explicit DataManager(std::string dataDirectory = "data");

    // Creates the data directory if it does not already exist. Returns
    // false if the directory could not be created.
    bool ensureDataDirectory() const;

    bool saveStatistics(const StatisticsManager& stats) const;
    bool loadStatistics(StatisticsManager& stats) const;

    bool saveHistory(const GameHistory& history) const;
    bool loadHistory(GameHistory& history) const;

    // Convenience wrappers that save/load everything in one call.
    bool saveAll(const StatisticsManager& stats, const GameHistory& history) const;
    bool loadAll(StatisticsManager& stats, GameHistory& history) const;

    std::string statisticsFilePath() const;
    std::string historyFilePath() const;

private:
    std::string dataDirectory_;
};

} // namespace rps
