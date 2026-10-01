// StatisticsManager.h
// Tracks per-player statistics using an unordered_map for O(1) average
// lookup by player name. This is the backend "database" of player
// performance that the Statistics and Profile screens read from.
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "core/Types.h"

namespace rps {

// Aggregated statistics for a single named player.
struct PlayerStats {
    std::string name;

    int matchesPlayed = 0;
    int matchesWon    = 0;
    int matchesLost   = 0;

    int roundsPlayed  = 0;
    int roundsWon     = 0;
    int roundsLost    = 0;
    int roundsDrawn   = 0;

    int currentStreak = 0; // consecutive match wins, resets on a loss
    int bestStreak    = 0;

    // Move frequency counters. Used to display a "favorite move" on the
    // profile screen and consumed by the Hard AI to predict the player.
    int rockCount     = 0;
    int paperCount    = 0;
    int scissorsCount = 0;

    double winRate() const {
        return matchesPlayed == 0 ? 0.0
             : (100.0 * static_cast<double>(matchesWon) / static_cast<double>(matchesPlayed));
    }

    Move favoriteMove() const {
        if (rockCount >= paperCount && rockCount >= scissorsCount) return Move::Rock;
        if (paperCount >= rockCount && paperCount >= scissorsCount) return Move::Paper;
        return Move::Scissors;
    }
};

class StatisticsManager {
public:
    // Returns a mutable reference to the stats for a player, creating a
    // fresh zeroed record if the player has never been seen before.
    PlayerStats& getOrCreate(const std::string& name);

    // Returns nullptr if the player has no recorded statistics.
    const PlayerStats* get(const std::string& name) const;

    // Updates move-frequency counters and round tallies for one round.
    void recordRound(const std::string& name, Move playerMove, RoundResult result);

    // Updates match-level counters (win/loss streaks, matches played).
    void recordMatchResult(const std::string& name, bool playerWonMatch);

    // Clears every player's statistics back to zero.
    void resetAll();

    // Clears statistics for a single player only.
    void resetPlayer(const std::string& name);

    // Read-only access to every tracked player, e.g. for the leaderboard.
    const std::unordered_map<std::string, PlayerStats>& all() const { return statsByName_; }

    // --- Persistence helpers (used by DataManager) ---
    // Serializes all stats into pipe-delimited lines, one per player.
    std::vector<std::string> serialize() const;
    // Rebuilds the map from lines previously produced by serialize().
    void deserialize(const std::vector<std::string>& lines);

private:
    std::unordered_map<std::string, PlayerStats> statsByName_;
};

} // namespace rps
