// AIPlayer.h
// Encapsulates the computer opponent's move-selection strategy. Kept as
// a stateless utility class (static methods only) since the decision
// only depends on the difficulty and the data passed in, not on any
// internal AI state.
#pragma once

#include "core/Types.h"
#include "core/StatisticsManager.h"

namespace rps {

class AIPlayer {
public:
    // Chooses the AI's move for the upcoming round.
    //   difficulty      - controls how "smart" the AI plays
    //   opponentStats   - the human player's historical stats, used by
    //                      Hard difficulty to predict their next move
    //                      (may be nullptr if no history exists yet)
    //   lastPlayerMove  - the move the human played in the previous
    //                      round of the current match
    //   hasLastMove     - false for the first round of a match, since
    //                      there is no previous move to react to
    static Move decideMove(Difficulty difficulty,
                            const PlayerStats* opponentStats,
                            Move lastPlayerMove,
                            bool hasLastMove);

private:
    // Returns a uniformly random move using the shared RNG engine.
    static Move randomMove();

    // Returns the move that defeats the given move (used to "counter").
    static Move counterTo(Move m);
};

} // namespace rps
