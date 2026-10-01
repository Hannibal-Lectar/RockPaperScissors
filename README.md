# Rock Paper Scissors — Desktop Edition

A polished, dark-themed desktop Rock Paper Scissors game built with modern C++17, Dear ImGui, GLFW, and OpenGL. The project features multiple match modes, AI difficulty levels, persistent player statistics, a leaderboard, match history, player profiles, and a graphical desktop interface.

## Features

- **Home Screen** — Enter a player name, choose match length and AI difficulty, and view a quick snapshot of player statistics.
- **Gameplay** — Rock, Paper, and Scissors controls with a live scoreboard, round-by-round log, Undo Last Move, and match-result popup.
- **Match Modes** — Best of 3, Best of 5, and Best of 7.
- **AI Difficulty**
  - Easy — Random move selection
  - Medium — Reactive AI
  - Hard — Predicts the player's most-used move using saved statistics
- **History Screen** — View completed matches and recent activity.
- **Statistics Dashboard** — Win rate, streaks, round breakdown, and move usage.
- **Leaderboard** — Players ranked by wins using a custom singly linked list.
- **Player Profiles** — Browse players, switch the active profile, and reset individual player statistics.
- **Save / Load** — Persistent statistics and match history with automatic loading on startup and saving on exit.
- **Modern Dark UI** — Custom Dear ImGui theme with rounded corners, accent colors, and color-coded win/loss/draw states.

## Technologies Used

- **C++17**
- **Dear ImGui**
- **GLFW**
- **OpenGL**
- **CMake**
- **File I/O**

## Data Structures Used

| Data Structure | Where | Purpose |
|---|---|---|
| `std::vector<MatchRecord>` | `GameHistory` | Stores the complete match history |
| `std::queue<MatchRecord>` | `GameHistory` | Maintains a rolling window of the 10 most recent matches |
| `std::stack<RoundRecord>` | `GameHistory` | Provides undo functionality for the current round |
| `std::unordered_map<std::string, PlayerStats>` | `StatisticsManager` | Provides average O(1) player-statistics lookup |
| Custom singly linked list | `Leaderboard` | Maintains players ranked by wins |

## Project Structure

```text
RockPaperScissors/
│
├── CMakeLists.txt
├── README.md
├── assets/
│   └── README.md
│
├── data/
│   └── README.md
│
├── include/
│   ├── core/
│   │   ├── Types.h
│   │   ├── AIPlayer.h
│   │   ├── GameEngine.h
│   │   ├── GameHistory.h
│   │   ├── StatisticsManager.h
│   │   ├── Leaderboard.h
│   │   └── DataManager.h
│   │
│   └── gui/
│       ├── Application.h
│       └── Theme.h
│
└── src/
    ├── main.cpp
    │
    ├── core/
    │   ├── Types.cpp
    │   ├── AIPlayer.cpp
    │   ├── GameEngine.cpp
    │   ├── GameHistory.cpp
    │   ├── StatisticsManager.cpp
    │   ├── Leaderboard.cpp
    │   └── DataManager.cpp
    │
    └── gui/
        ├── Application.cpp
        ├── Theme.cpp
        └── screens/
            ├── HomeScreen.cpp
            ├── GameScreen.cpp
            ├── HistoryScreen.cpp
            ├── StatisticsScreen.cpp
            ├── LeaderboardScreen.cpp
            └── ProfileScreen.cpp
