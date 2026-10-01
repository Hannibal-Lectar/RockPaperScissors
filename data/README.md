# Data Folder

This folder holds the application's persistent save files. They are
created automatically the first time you save from the app, and are
plain, human-readable, pipe-delimited text files (no external database
or JSON library required).

- `statistics.dat` - one line per player: aggregated match/round
  counters and move-frequency counts used to compute stats and the
  leaderboard.
- `history.dat` - one line per completed match, including every round
  played in that match.

You normally never need to edit these by hand. Deleting both files (or
using **Reset All Statistics** inside the app) returns the app to a
fresh state.

The two files in this folder are intentionally shipped empty so the
application starts with a clean slate the first time you run it.
