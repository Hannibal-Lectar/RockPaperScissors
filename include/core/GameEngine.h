// GameEngine.h
// Owns the state of the match currently being played: score, round
// number, and the sequence of rounds played so far. This is the main
// backend class the GUI's GameScreen talks to; it knows nothing about
// ImGui or rendering.
#pragma once

#include <string>
#include <vector>

#include "core/Types.h"
#include "core/GameHistory.h"
#include "core/StatisticsManager.h"

namespace rps {

class GameEngine {
public:
    // Begins a brand new match, resetting all in-progress state.
    void startMatch(const std::string& playerName, MatchMode mode, Difficulty difficulty);

    // Plays one round given the player's chosen move. Internally asks
    // AIPlayer for its move, evaluates the round, updates the score,
    // and returns the resulting RoundRecord for the caller to display
    // and (optionally) push onto the undo stack.
    //
    //   opponentStats - the player's historical stats, forwarded to the
    //                    AI so Hard difficulty can use them; may be
    //                    nullptr.
    RoundRecord playRound(Move playerMove, const PlayerStats* opponentStats);

    // Reverts the engine's score/round-number state by one round, given
    // the RoundRecord that was undone (typically popped from
    // GameHistory's undo stack). Does nothing if no round has been
    // played yet in the current match.
    void undoRound(const RoundRecord& round);

    bool isMatchOver() const;
    bool didPlayerWinMatch() const;

    // Packages the current match state into a MatchRecord, stamped with
    // the current time and the next available match id. Does not reset
    // engine state -- call startMatch() again to begin a new match.
    MatchRecord buildMatchRecord(int matchId) const;

    bool isActive() const { return matchActive_; }
    int getPlayerScore() const { return playerScore_; }
    int getAiScore() const { return aiScore_; }
    int getRoundNumber() const { return roundNumber_; }
    int getWinsNeeded() const { return winsNeeded_; }
    MatchMode getMode() const { return mode_; }
    Difficulty getDifficulty() const { return difficulty_; }
    const std::string& getPlayerName() const { return playerName_; }
    const std::vector<RoundRecord>& getCurrentRounds() const { return currentRounds_; }

private:
    std::string playerName_;
    MatchMode mode_ = MatchMode::BestOf3;
    Difficulty difficulty_ = Difficulty::Medium;

    int playerScore_ = 0;
    int aiScore_ = 0;
    int roundNumber_ = 0;
    int winsNeeded_ = 2;

    Move lastPlayerMove_ = Move::Rock;
    bool hasLastMove_ = false;

    bool matchActive_ = false;

    std::vector<RoundRecord> currentRounds_;
};

} // namespace rps
