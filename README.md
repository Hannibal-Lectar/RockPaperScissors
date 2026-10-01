# Rock Paper Scissors — Desktop Edition

A polished, dark-themed desktop Rock, Paper, Scissors game built in
modern C++17 with a real graphical interface (Dear ImGui + GLFW +
OpenGL), clean separation between game logic and UI, persistent player
statistics, a leaderboard, and full match history.

---

## Features

- **Home screen** — enter a player name, choose match length and AI
  difficulty, see a quick snapshot of your stats.
- **Gameplay** — Rock / Paper / Scissors buttons, live scoreboard,
  round-by-round log, **Undo Last Move**, and a match-result popup.
- **Match modes** — Best of 3, Best of 5, Best of 7.
- **Difficulty levels** — Easy (random), Medium (reactive), Hard
  (predicts your most-used move from saved statistics).
- **History screen** — full match log plus a "recent activity" strip.
- **Statistics dashboard** — win rate, streaks, round breakdown, move
  usage, with a confirm-before-you-wipe reset.
- **Leaderboard** — every player ranked by wins, rendered from a
  hand-written linked list.
- **Player profiles** — browse any player who has played, switch the
  active profile, or reset a single player's stats.
- **Save / Load** — one click to persist everything to disk, and data
  is loaded automatically on startup and saved automatically on exit.
- **Modern dark UI** — custom ImGui theme (rounded corners, accent
  colors, color-coded win/loss/draw states).

## Data Structures Used (backend)

| Structure                    | Where                          | Purpose                                              |
|-------------------------------|---------------------------------|-------------------------------------------------------|
| `std::vector<MatchRecord>`    | `GameHistory`                  | Full permanent match log                              |
| `std::queue<MatchRecord>`     | `GameHistory`                  | Rolling window of the 10 most recent matches           |
| `std::stack<RoundRecord>`     | `GameHistory`                  | Undo support for the round in progress                 |
| `std::unordered_map<string, PlayerStats>` | `StatisticsManager` | O(1) average lookup of per-player statistics |
| Custom singly linked list (`Leaderboard`) | `Leaderboard.h/.cpp` | Players ranked by wins, kept sorted on insert |

## Project Structure

```
RockPaperScissors/
├── CMakeLists.txt              # Build configuration (fetches GLFW + Dear ImGui)
├── README.md
├── assets/                     # Runtime assets (icons/fonts if you add them)
├── data/                       # Persistent save files (statistics.dat, history.dat)
├── include/
│   ├── core/                   # Backend headers (no GUI dependencies)
│   │   ├── Types.h
│   │   ├── AIPlayer.h
│   │   ├── GameEngine.h
│   │   ├── GameHistory.h
│   │   ├── StatisticsManager.h
│   │   ├── Leaderboard.h
│   │   └── DataManager.h
│   └── gui/                    # Frontend headers
│       ├── Application.h
│       └── Theme.h
└── src/
    ├── main.cpp
    ├── core/                   # Backend implementation (game rules, AI, persistence)
    │   ├── Types.cpp
    │   ├── AIPlayer.cpp
    │   ├── GameEngine.cpp
    │   ├── GameHistory.cpp
    │   ├── StatisticsManager.cpp
    │   ├── Leaderboard.cpp
    │   └── DataManager.cpp
    └── gui/                    # Frontend implementation
        ├── Application.cpp
        ├── Theme.cpp
        └── screens/
            ├── HomeScreen.cpp
            ├── GameScreen.cpp
            ├── HistoryScreen.cpp
            ├── StatisticsScreen.cpp
            ├── LeaderboardScreen.cpp
            └── ProfileScreen.cpp
```

The `core/` module (backend) has **no knowledge of ImGui, GLFW, or
OpenGL** — it is pure C++17 game logic and file I/O, and could be
reused for a console version or unit tested independently. The `gui/`
module (frontend) depends on `core/` but not the other way around.

## Requirements

- **CMake 3.16+**
- A **C++17** compiler:
  - Windows: Visual Studio 2019/2022 (MSVC) or MinGW-w64
  - macOS: Xcode Command Line Tools (Clang)
  - Linux: GCC or Clang
- **An internet connection the first time you configure the project.**
  CMake's `FetchContent` automatically downloads GLFW and Dear ImGui
  source code from GitHub at configure time — you do not need to
  install these libraries yourself. Once cached, subsequent builds do
  not need the internet again.
- **OpenGL** development headers/libraries, which ship with the OS on
  Windows and macOS. On Linux you may need your distro's OpenGL/X11
  development packages, e.g. on Ubuntu/Debian:
  ```bash
  sudo apt update
  sudo apt install build-essential cmake xorg-dev libgl1-mesa-dev
  ```

## Building

### Windows (Visual Studio)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The executable will be at `build\Release\RockPaperScissors.exe`.

### Windows (MinGW-w64)

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --config Release
```

### macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The executable will be at `build/RockPaperScissors`.

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(nproc)
```

## Running

From the build output directory, just run the executable:

```bash
# macOS / Linux
./build/RockPaperScissors

# Windows
build\Release\RockPaperScissors.exe
```

The build automatically copies the `data/` and `assets/` folders next
to the compiled executable, so save/load works correctly regardless of
which directory you launch it from.

## Using the App

1. On **Home**, type a player name, pick a match length and
   difficulty, then click **Start Game**.
2. On the **Play** screen, click **Rock**, **Paper**, or **Scissors**
   each round. Use **Undo Last Move** if you misclick.
3. When the match ends, a popup lets you **Play Again** or go **Back
   to Home**.
4. Check **History** for every past match, **Statistics** for your
   personal dashboard, **Leaderboard** for the overall ranking, and
   **Profile** to browse or reset any individual player.
5. Click **Save** any time to persist progress immediately (the app
   also auto-saves on exit and auto-loads on startup).

## Notes on This Build

This project was authored to compile cleanly with the toolchains
listed above, following the standard Dear ImGui + GLFW + OpenGL3
integration pattern used in Dear ImGui's own official examples
(`examples/example_glfw_opengl3`). It was written in a sandboxed
environment without internet access or a display, so it could not be
compiled and run end-to-end here — if you hit a build error, it is
most likely a toolchain/version mismatch rather than a structural
issue, and the CMake output plus this README should make it quick to
diagnose.

## License

This sample project is provided as-is for learning and portfolio use.
