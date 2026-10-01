// GameEngine.cpp
#include "core/GameEngine.h"
#include "core/AIPlayer.h"

namespace rps {

void GameEngine::startMatch(const std::string& playerName, MatchMode mode, Difficulty difficulty) {
    playerName_ = playerName;
    mode_ = mode;
    difficulty_ = difficulty;

    playerScore_ = 0;
    aiScore_ = 0;
    roundNumber_ = 0;
    winsNeeded_ = winsRequiredForMode(mode);

    hasLastMove_ = false;
    matchActive_ = true;

    currentRounds_.clear();
}

RoundRecord GameEngine::playRound(Move playerMove, const PlayerStats* opponentStats) {
    Move aiMove = AIPlayer::decideMove(difficulty_, opponentStats, lastPlayerMove_, hasLastMove_);
    RoundResult result = evaluateRound(playerMove, aiMove);

    roundNumber_++;

    if (result == RoundResult::Win) {
        playerScore_++;
    } else if (result == RoundResult::Lose) {
        aiScore_++;
    }

    lastPlayerMove_ = playerMove;
    hasLastMove_ = true;

    RoundRecord record;
    record.roundNumber = roundNumber_;
    record.playerMove = playerMove;
    record.aiMove = aiMove;
    record.result = result;

    currentRounds_.push_back(record);
    return record;
}

void GameEngine::undoRound(const RoundRecord& round) {
    if (currentRounds_.empty()) {
        return;
    }

    // Only allow undoing the most recently played round, to keep the
    // round sequence consistent.
    if (currentRounds_.back().roundNumber != round.roundNumber) {
        return;
    }

    if (round.result == RoundResult::Win) {
        playerScore_--;
    } else if (round.result == RoundResult::Lose) {
        aiScore_--;
    }

    currentRounds_.pop_back();
    roundNumber_--;

    if (!currentRounds_.empty()) {
        const RoundRecord& previous = currentRounds_.back();
        lastPlayerMove_ = previous.playerMove;
        hasLastMove_ = true;
    } else {
        hasLastMove_ = false;
    }
}

bool GameEngine::isMatchOver() const {
    return playerScore_ >= winsNeeded_ || aiScore_ >= winsNeeded_;
}

bool GameEngine::didPlayerWinMatch() const {
    return playerScore_ >= winsNeeded_;
}

MatchRecord GameEngine::buildMatchRecord(int matchId) const {
    MatchRecord record;
    record.matchId = matchId;
    record.playerName = playerName_;
    record.mode = mode_;
    record.difficulty = difficulty_;
    record.rounds = currentRounds_;
    record.playerScore = playerScore_;
    record.aiScore = aiScore_;
    record.playerWonMatch = didPlayerWinMatch();
    record.timestamp = currentTimestamp();
    return record;
}

} // namespace rps
