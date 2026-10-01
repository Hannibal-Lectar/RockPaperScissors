// AIPlayer.cpp
#include "core/AIPlayer.h"

#include <random>
#include <array>

namespace rps {

namespace {
    // A single shared random engine, seeded once, reused for every roll
    // so we are not constantly re-seeding (which would bias results).
    std::mt19937& rng() {
        static std::mt19937 engine(std::random_device{}());
        return engine;
    }

    int rollPercent() {
        static std::uniform_int_distribution<int> dist(1, 100);
        return dist(rng());
    }
}

Move AIPlayer::randomMove() {
    static std::uniform_int_distribution<int> dist(0, 2);
    return static_cast<Move>(dist(rng()));
}

Move AIPlayer::counterTo(Move m) {
    switch (m) {
        case Move::Rock:     return Move::Paper;    // Paper beats Rock
        case Move::Paper:    return Move::Scissors;  // Scissors beats Paper
        case Move::Scissors: return Move::Rock;      // Rock beats Scissors
    }
    return Move::Rock;
}

Move AIPlayer::decideMove(Difficulty difficulty,
                           const PlayerStats* opponentStats,
                           Move lastPlayerMove,
                           bool hasLastMove) {
    switch (difficulty) {
        case Difficulty::Easy: {
            // Pure chance. Gives new players an easy, beatable opponent.
            return randomMove();
        }

        case Difficulty::Medium: {
            // 55% of the time, react to the player's previous move by
            // countering it (a naive but noticeable pattern). Otherwise
            // play randomly. This makes Medium feel smarter than Easy
            // without being unbeatable.
            if (hasLastMove && rollPercent() <= 55) {
                return counterTo(lastPlayerMove);
            }
            return randomMove();
        }

        case Difficulty::Hard: {
            // Predict the player's most frequently played move overall
            // (from persisted statistics) and counter it. Falls back to
            // countering the last move, then to random, when there is
            // not enough data yet. A small amount of randomness (15%)
            // is kept so Hard is very tough but not perfectly
            // deterministic/exploitable.
            if (rollPercent() <= 15) {
                return randomMove();
            }
            if (opponentStats != nullptr && opponentStats->roundsPlayed > 0) {
                return counterTo(opponentStats->favoriteMove());
            }
            if (hasLastMove) {
                return counterTo(lastPlayerMove);
            }
            return randomMove();
        }
    }
    return randomMove();
}

} // namespace rps
