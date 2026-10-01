// Application.h
// Owns the GLFW window, the ImGui context, and every piece of backend
// state (game engine, statistics, history, leaderboard, persistence).
// The individual renderXScreen() methods are implemented in separate
// .cpp files under src/gui/screens/ to keep each screen's code
// self-contained while still sharing state through this class.
#pragma once

#include <string>
#include <vector>

struct GLFWwindow;

#include "core/Types.h"
#include "core/GameEngine.h"
#include "core/GameHistory.h"
#include "core/StatisticsManager.h"
#include "core/Leaderboard.h"
#include "core/DataManager.h"

namespace rps::gui {

class Application {
public:
    Application();
    ~Application();

    // Creates the GLFW window, OpenGL context and ImGui context.
    // Returns false if any step fails.
    bool initialize();

    // Runs the main loop until the window is closed.
    void run();

    // Tears down ImGui, GLFW and releases the window.
    void shutdown();

private:
    // --- Window / lifecycle ---
    GLFWwindow* window_ = nullptr;
    bool initialized_ = false;

    void renderFrame();
    void renderTopNavigation();

    // --- Backend state, shared by every screen ---
    rps::Screen currentScreen_ = rps::Screen::Home;
    rps::GameEngine engine_;
    rps::GameHistory history_;
    rps::StatisticsManager statsManager_;
    rps::Leaderboard leaderboard_;
    rps::DataManager dataManager_;

    // --- Shared UI state (persists as the player navigates screens) ---
    char playerNameBuffer_[64] = "Player1";
    rps::MatchMode selectedMode_ = rps::MatchMode::BestOf3;
    rps::Difficulty selectedDifficulty_ = rps::Difficulty::Medium;

    std::string statusMessage_;
    double statusMessageTimer_ = 0.0;

    bool hasLastRoundResult_ = false;
    rps::RoundRecord lastRoundResult_;

    bool matchJustEnded_ = false;
    rps::MatchRecord lastFinishedMatch_;

    std::string profileSelectedPlayer_;

    // --- Screen render functions (defined in src/gui/screens/*.cpp) ---
    void renderHomeScreen();
    void renderGameScreen();
    void renderHistoryScreen();
    void renderStatisticsScreen();
    void renderLeaderboardScreen();
    void renderProfileScreen();

    // --- Shared helper logic used by multiple screens ---
    void startNewMatch();
    void handlePlayerMove(rps::Move move);
    void handleUndoMove();
    void finishCurrentMatch();
    void refreshLeaderboardFromStats();
    void showStatusMessage(const std::string& message);
    void saveAllData();
    void loadAllData();
    void resetAllStatistics();

    std::string currentPlayerName() const { return std::string(playerNameBuffer_); }
};

} // namespace rps::gui
