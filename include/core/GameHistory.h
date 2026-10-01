// GameHistory.h
// Records completed rounds and matches, and provides an "undo" facility
// for the round currently in progress.
//
// Data structures used here (as required by the project spec):
//   - std::vector<MatchRecord>  : full permanent match log, used by the
//                                  History screen to show everything.
//   - std::queue<MatchRecord>   : a rolling window of the most recent
//                                  matches, used for a quick "recent
//                                  activity" view without scanning the
//                                  whole vector.
//   - std::stack<RoundRecord>   : the rounds played so far in the match
//                                  that is currently in progress, so the
//                                  player can undo their last move.
#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <string>

#include "core/Types.h"

namespace rps {

// A single round within a match.
struct RoundRecord {
    int roundNumber = 0;
    Move playerMove = Move::Rock;
    Move aiMove     = Move::Rock;
    RoundResult result = RoundResult::Draw;
};

// A completed (or in-progress) match, made up of individual rounds.
struct MatchRecord {
    int matchId = 0;
    std::string playerName;
    MatchMode mode = MatchMode::BestOf3;
    Difficulty difficulty = Difficulty::Medium;
    std::vector<RoundRecord> rounds;
    int playerScore = 0;
    int aiScore = 0;
    bool playerWonMatch = false;
    std::string timestamp;
};

class GameHistory {
public:
    // Adds a finished match to the permanent log and to the recent-
    // matches queue (evicting the oldest entry if the queue is full).
    void recordMatch(const MatchRecord& match);

    // --- Undo stack, scoped to the match currently being played ---
    void pushRoundForUndo(const RoundRecord& round);
    // Pops the most recent round off the undo stack into `out`.
    // Returns false if there was nothing to undo.
    bool undoLastRound(RoundRecord& out);
    void clearUndoStack();
    bool canUndo() const { return !undoStack_.empty(); }

    // --- Read access ---
    const std::vector<MatchRecord>& getAllMatches() const { return matchLog_; }
    // Returns the recent-matches queue contents as a vector (newest last)
    // without disturbing the underlying queue.
    std::vector<MatchRecord> getRecentMatches() const;

    void clearHistory();
    int nextMatchId() const { return static_cast<int>(matchLog_.size()) + 1; }

    // --- Persistence helpers (used by DataManager) ---
    std::vector<std::string> serialize() const;
    void deserialize(const std::vector<std::string>& lines);

private:
    static constexpr size_t kMaxRecentMatches = 10;

    std::vector<MatchRecord> matchLog_;
    std::queue<MatchRecord> recentMatches_;
    std::stack<RoundRecord> undoStack_;
};

} // namespace rps
