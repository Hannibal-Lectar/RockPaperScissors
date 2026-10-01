// StatisticsManager.cpp
#include "core/StatisticsManager.h"

#include <sstream>

namespace rps {

PlayerStats& StatisticsManager::getOrCreate(const std::string& name) {
    auto it = statsByName_.find(name);
    if (it != statsByName_.end()) {
        return it->second;
    }
    PlayerStats fresh;
    fresh.name = name;
    auto result = statsByName_.emplace(name, fresh);
    return result.first->second;
}

const PlayerStats* StatisticsManager::get(const std::string& name) const {
    auto it = statsByName_.find(name);
    return it == statsByName_.end() ? nullptr : &it->second;
}

void StatisticsManager::recordRound(const std::string& name, Move playerMove, RoundResult result) {
    PlayerStats& stats = getOrCreate(name);
    stats.roundsPlayed++;

    switch (result) {
        case RoundResult::Win:  stats.roundsWon++;   break;
        case RoundResult::Lose: stats.roundsLost++;  break;
        case RoundResult::Draw: stats.roundsDrawn++; break;
    }

    switch (playerMove) {
        case Move::Rock:     stats.rockCount++;     break;
        case Move::Paper:    stats.paperCount++;    break;
        case Move::Scissors: stats.scissorsCount++; break;
    }
}

void StatisticsManager::recordMatchResult(const std::string& name, bool playerWonMatch) {
    PlayerStats& stats = getOrCreate(name);
    stats.matchesPlayed++;

    if (playerWonMatch) {
        stats.matchesWon++;
        stats.currentStreak++;
        if (stats.currentStreak > stats.bestStreak) {
            stats.bestStreak = stats.currentStreak;
        }
    } else {
        stats.matchesLost++;
        stats.currentStreak = 0;
    }
}

void StatisticsManager::resetAll() {
    statsByName_.clear();
}

void StatisticsManager::resetPlayer(const std::string& name) {
    auto it = statsByName_.find(name);
    if (it != statsByName_.end()) {
        PlayerStats fresh;
        fresh.name = name;
        it->second = fresh;
    }
}

std::vector<std::string> StatisticsManager::serialize() const {
    std::vector<std::string> lines;
    lines.reserve(statsByName_.size());
    for (const auto& [name, s] : statsByName_) {
        std::ostringstream oss;
        oss << s.name << '|'
            << s.matchesPlayed << '|' << s.matchesWon << '|' << s.matchesLost << '|'
            << s.roundsPlayed  << '|' << s.roundsWon  << '|' << s.roundsLost  << '|' << s.roundsDrawn << '|'
            << s.currentStreak << '|' << s.bestStreak << '|'
            << s.rockCount << '|' << s.paperCount << '|' << s.scissorsCount;
        lines.push_back(oss.str());
    }
    return lines;
}

void StatisticsManager::deserialize(const std::vector<std::string>& lines) {
    statsByName_.clear();
    for (const auto& line : lines) {
        if (line.empty()) continue;

        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }
        if (fields.size() < 13) continue; // corrupted / malformed line, skip

        PlayerStats s;
        s.name           = fields[0];
        s.matchesPlayed  = std::stoi(fields[1]);
        s.matchesWon     = std::stoi(fields[2]);
        s.matchesLost    = std::stoi(fields[3]);
        s.roundsPlayed   = std::stoi(fields[4]);
        s.roundsWon      = std::stoi(fields[5]);
        s.roundsLost     = std::stoi(fields[6]);
        s.roundsDrawn    = std::stoi(fields[7]);
        s.currentStreak  = std::stoi(fields[8]);
        s.bestStreak     = std::stoi(fields[9]);
        s.rockCount      = std::stoi(fields[10]);
        s.paperCount     = std::stoi(fields[11]);
        s.scissorsCount  = std::stoi(fields[12]);

        statsByName_[s.name] = s;
    }
}

} // namespace rps
