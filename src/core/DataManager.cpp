// DataManager.cpp
#include "core/DataManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace rps {

DataManager::DataManager(std::string dataDirectory)
    : dataDirectory_(std::move(dataDirectory)) {}

bool DataManager::ensureDataDirectory() const {
    std::error_code ec;
    if (fs::exists(dataDirectory_, ec)) {
        return true;
    }
    return fs::create_directories(dataDirectory_, ec);
}

std::string DataManager::statisticsFilePath() const {
    return (fs::path(dataDirectory_) / "statistics.dat").string();
}

std::string DataManager::historyFilePath() const {
    return (fs::path(dataDirectory_) / "history.dat").string();
}

bool DataManager::saveStatistics(const StatisticsManager& stats) const {
    if (!ensureDataDirectory()) {
        return false;
    }
    std::ofstream file(statisticsFilePath(), std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    for (const auto& line : stats.serialize()) {
        file << line << '\n';
    }
    return true;
}

bool DataManager::loadStatistics(StatisticsManager& stats) const {
    std::ifstream file(statisticsFilePath());
    if (!file.is_open()) {
        // No save file yet is not an error -- just start fresh.
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    stats.deserialize(lines);
    return true;
}

bool DataManager::saveHistory(const GameHistory& history) const {
    if (!ensureDataDirectory()) {
        return false;
    }
    std::ofstream file(historyFilePath(), std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    for (const auto& line : history.serialize()) {
        file << line << '\n';
    }
    return true;
}

bool DataManager::loadHistory(GameHistory& history) const {
    std::ifstream file(historyFilePath());
    if (!file.is_open()) {
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    history.deserialize(lines);
    return true;
}

bool DataManager::saveAll(const StatisticsManager& stats, const GameHistory& history) const {
    bool statsOk = saveStatistics(stats);
    bool historyOk = saveHistory(history);
    return statsOk && historyOk;
}

bool DataManager::loadAll(StatisticsManager& stats, GameHistory& history) const {
    bool statsOk = loadStatistics(stats);
    bool historyOk = loadHistory(history);
    return statsOk || historyOk; // true if at least one file existed
}

} // namespace rps
