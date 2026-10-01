// Types.cpp
#include "core/Types.h"

#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace rps {

std::string moveToString(Move m) {
    switch (m) {
        case Move::Rock:     return "Rock";
        case Move::Paper:    return "Paper";
        case Move::Scissors: return "Scissors";
    }
    return "Unknown";
}

Move moveFromString(const std::string& s) {
    if (s == "Rock")     return Move::Rock;
    if (s == "Paper")    return Move::Paper;
    if (s == "Scissors") return Move::Scissors;
    // Fall back to Rock for corrupted/unknown data rather than throwing,
    // so a damaged save file never crashes the application.
    return Move::Rock;
}

std::string resultToString(RoundResult r) {
    switch (r) {
        case RoundResult::Win:  return "Win";
        case RoundResult::Lose: return "Lose";
        case RoundResult::Draw: return "Draw";
    }
    return "Unknown";
}

RoundResult resultFromString(const std::string& s) {
    if (s == "Win")  return RoundResult::Win;
    if (s == "Lose") return RoundResult::Lose;
    return RoundResult::Draw;
}

std::string difficultyToString(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:   return "Easy";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Hard:   return "Hard";
    }
    return "Unknown";
}

Difficulty difficultyFromString(const std::string& s) {
    if (s == "Easy")   return Difficulty::Easy;
    if (s == "Hard")   return Difficulty::Hard;
    return Difficulty::Medium;
}

std::string modeToString(MatchMode m) {
    switch (m) {
        case MatchMode::BestOf3: return "Best of 3";
        case MatchMode::BestOf5: return "Best of 5";
        case MatchMode::BestOf7: return "Best of 7";
    }
    return "Unknown";
}

MatchMode modeFromString(const std::string& s) {
    if (s == "Best of 3") return MatchMode::BestOf3;
    if (s == "Best of 7") return MatchMode::BestOf7;
    return MatchMode::BestOf5;
}

int winsRequiredForMode(MatchMode mode) {
    // Best of N requires ceil((N+1)/2) wins, i.e. a strict majority.
    int totalRounds = static_cast<int>(mode);
    return (totalRounds / 2) + 1;
}

RoundResult evaluateRound(Move player, Move ai) {
    if (player == ai) {
        return RoundResult::Draw;
    }
    // Rock beats Scissors, Paper beats Rock, Scissors beats Paper.
    bool playerWins =
        (player == Move::Rock     && ai == Move::Scissors) ||
        (player == Move::Paper    && ai == Move::Rock)      ||
        (player == Move::Scissors && ai == Move::Paper);

    return playerWins ? RoundResult::Win : RoundResult::Lose;
}

std::string currentTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm tmStruct{};
#if defined(_WIN32)
    localtime_s(&tmStruct, &t);
#else
    localtime_r(&t, &tmStruct);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

} // namespace rps
