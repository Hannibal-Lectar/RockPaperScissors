// GameHistory.cpp
#include "core/GameHistory.h"

#include <sstream>
#include <queue>

namespace rps {

void GameHistory::recordMatch(const MatchRecord& match) {
    matchLog_.push_back(match);

    recentMatches_.push(match);
    while (recentMatches_.size() > kMaxRecentMatches) {
        recentMatches_.pop();
    }
}

void GameHistory::pushRoundForUndo(const RoundRecord& round) {
    undoStack_.push(round);
}

bool GameHistory::undoLastRound(RoundRecord& out) {
    if (undoStack_.empty()) {
        return false;
    }
    out = undoStack_.top();
    undoStack_.pop();
    return true;
}

void GameHistory::clearUndoStack() {
    // std::stack has no clear(); swap with an empty stack instead.
    std::stack<RoundRecord> empty;
    std::swap(undoStack_, empty);
}

std::vector<MatchRecord> GameHistory::getRecentMatches() const {
    // Copy the queue so we can drain it without touching the original.
    std::queue<MatchRecord> copy = recentMatches_;
    std::vector<MatchRecord> result;
    result.reserve(copy.size());
    while (!copy.empty()) {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}

void GameHistory::clearHistory() {
    matchLog_.clear();
    std::queue<MatchRecord> emptyQueue;
    std::swap(recentMatches_, emptyQueue);
    clearUndoStack();
}

namespace {
    std::string serializeRounds(const std::vector<RoundRecord>& rounds) {
        std::ostringstream oss;
        for (size_t i = 0; i < rounds.size(); ++i) {
            const RoundRecord& r = rounds[i];
            oss << r.roundNumber << ','
                << moveToString(r.playerMove) << ','
                << moveToString(r.aiMove) << ','
                << resultToString(r.result);
            if (i + 1 < rounds.size()) oss << ';';
        }
        return oss.str();
    }

    std::vector<RoundRecord> deserializeRounds(const std::string& blob) {
        std::vector<RoundRecord> rounds;
        std::stringstream roundsStream(blob);
        std::string roundToken;
        while (std::getline(roundsStream, roundToken, ';')) {
            if (roundToken.empty()) continue;
            std::stringstream fieldStream(roundToken);
            std::string field;
            std::vector<std::string> fields;
            while (std::getline(fieldStream, field, ',')) {
                fields.push_back(field);
            }
            if (fields.size() < 4) continue;

            RoundRecord r;
            r.roundNumber = std::stoi(fields[0]);
            r.playerMove  = moveFromString(fields[1]);
            r.aiMove      = moveFromString(fields[2]);
            r.result      = resultFromString(fields[3]);
            rounds.push_back(r);
        }
        return rounds;
    }
}

std::vector<std::string> GameHistory::serialize() const {
    std::vector<std::string> lines;
    lines.reserve(matchLog_.size());
    for (const auto& m : matchLog_) {
        std::ostringstream oss;
        oss << m.matchId << '|'
            << m.playerName << '|'
            << modeToString(m.mode) << '|'
            << difficultyToString(m.difficulty) << '|'
            << m.playerScore << '|'
            << m.aiScore << '|'
            << (m.playerWonMatch ? 1 : 0) << '|'
            << m.timestamp << '|'
            << serializeRounds(m.rounds);
        lines.push_back(oss.str());
    }
    return lines;
}

void GameHistory::deserialize(const std::vector<std::string>& lines) {
    clearHistory();
    for (const auto& line : lines) {
        if (line.empty()) continue;

        // Split on '|' but keep the final field (round data / timestamp,
        // which themselves may be empty) intact.
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }
        if (fields.size() < 8) continue; // malformed, skip

        MatchRecord m;
        m.matchId       = std::stoi(fields[0]);
        m.playerName    = fields[1];
        m.mode          = modeFromString(fields[2]);
        m.difficulty    = difficultyFromString(fields[3]);
        m.playerScore   = std::stoi(fields[4]);
        m.aiScore       = std::stoi(fields[5]);
        m.playerWonMatch = fields[6] == "1";
        m.timestamp     = fields[7];
        if (fields.size() >= 9) {
            m.rounds = deserializeRounds(fields[8]);
        }

        matchLog_.push_back(m);
        recentMatches_.push(m);
        while (recentMatches_.size() > kMaxRecentMatches) {
            recentMatches_.pop();
        }
    }
}

} // namespace rps
