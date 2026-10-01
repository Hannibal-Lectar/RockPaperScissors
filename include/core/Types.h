// Types.h
// Core enumerations and small free functions shared across the backend.
// Keeping these in one place avoids circular includes between the
// game logic classes (GameEngine, AIPlayer, GameHistory, etc).
#pragma once

#include <string>

namespace rps {

// The three possible moves in a round of Rock Paper Scissors.
enum class Move {
    Rock = 0,
    Paper = 1,
    Scissors = 2
};

// Outcome of a single round, always expressed from the human player's
// point of view (Win means the player beat the AI that round).
enum class RoundResult {
    Win,
    Lose,
    Draw
};

// Difficulty of the computer opponent. Determines how the AI selects
// its next move (see AIPlayer).
enum class Difficulty {
    Easy,
    Medium,
    Hard
};

// Match length. The underlying integer is the number of rounds that
// make up the match (first to win the majority takes the match).
enum class MatchMode {
    BestOf3 = 3,
    BestOf5 = 5,
    BestOf7 = 7
};

// Which screen of the application is currently being displayed.
enum class Screen {
    Home,
    Game,
    History,
    Statistics,
    Leaderboard,
    Profile
};

// ---- Free helper functions -------------------------------------------

// Human readable text for a move, e.g. "Rock".
std::string moveToString(Move m);

// Converts a string produced by moveToString back into a Move. Used
// when loading saved data from disk.
Move moveFromString(const std::string& s);

// Human readable text for a round result, e.g. "Win".
std::string resultToString(RoundResult r);
RoundResult resultFromString(const std::string& s);

// Human readable text for a difficulty level.
std::string difficultyToString(Difficulty d);
Difficulty difficultyFromString(const std::string& s);

// Human readable text for a match mode, e.g. "Best of 5".
std::string modeToString(MatchMode m);
MatchMode modeFromString(const std::string& s);

// Returns how many round wins are required to win a match of the
// given mode (majority of the total rounds).
int winsRequiredForMode(MatchMode mode);

// Core game rule: evaluates the outcome of one round from the
// player's perspective given the player's move and the AI's move.
RoundResult evaluateRound(Move player, Move ai);

// Returns a timestamp string ("YYYY-MM-DD HH:MM:SS") for the current
// local time. Used to stamp match records when they are recorded.
std::string currentTimestamp();

} // namespace rps
