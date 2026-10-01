// Application.cpp
#include "gui/Application.h"
#include "gui/Theme.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace rps::gui {

namespace {
    void glfwErrorCallback(int error, const char* description) {
        std::cerr << "[GLFW Error " << error << "] " << description << std::endl;
    }
}

Application::Application() : dataManager_("data") {}

Application::~Application() {
    if (initialized_) {
        shutdown();
    }
}

bool Application::initialize() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Request an OpenGL 3.3 core profile context, which is the
    // baseline Dear ImGui's OpenGL3 backend targets.
#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    const char* glslVersion = "#version 150";
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const char* glslVersion = "#version 130";
#endif

    window_ = glfwCreateWindow(1280, 800, "Rock Paper Scissors", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // A slightly larger default font makes the UI feel more like a
    // polished desktop app rather than a debug overlay.
    ImFontConfig fontConfig;
    fontConfig.SizePixels = 19.0f;
    io.Fonts->AddFontDefault(&fontConfig);

    applyDarkTheme();

    if (!ImGui_ImplGlfw_InitForOpenGL(window_, true)) {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init(glslVersion)) {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend" << std::endl;
        return false;
    }

    // Load any previously saved statistics/history so the app resumes
    // right where the player left off.
    loadAllData();
    refreshLeaderboardFromStats();

    initialized_ = true;
    return true;
}

void Application::run() {
    while (window_ != nullptr && !glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderFrame();

        ImGui::Render();

        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window_, &displayWidth, &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(0.09f, 0.10f, 0.13f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);

        // Fade out transient status messages ("Saved!", "Loaded!", etc.)
        if (statusMessageTimer_ > 0.0) {
            statusMessageTimer_ -= 1.0 / 60.0;
            if (statusMessageTimer_ <= 0.0) {
                statusMessage_.clear();
            }
        }
    }
}

void Application::shutdown() {
    // Persist progress automatically on exit so the player never loses
    // stats by forgetting to click "Save".
    saveAllData();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window_ != nullptr) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
    initialized_ = false;
}

void Application::renderFrame() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("RootWindow", nullptr, flags);

    renderTopNavigation();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    switch (currentScreen_) {
        case rps::Screen::Home:        renderHomeScreen();        break;
        case rps::Screen::Game:        renderGameScreen();        break;
        case rps::Screen::History:     renderHistoryScreen();     break;
        case rps::Screen::Statistics:  renderStatisticsScreen();  break;
        case rps::Screen::Leaderboard: renderLeaderboardScreen(); break;
        case rps::Screen::Profile:     renderProfileScreen();     break;
    }

    ImGui::End();
}

void Application::renderTopNavigation() {
    struct NavItem { const char* label; rps::Screen screen; };
    static const NavItem items[] = {
        {"Home",        rps::Screen::Home},
        {"Play",        rps::Screen::Game},
        {"History",     rps::Screen::History},
        {"Statistics",  rps::Screen::Statistics},
        {"Leaderboard", rps::Screen::Leaderboard},
        {"Profile",     rps::Screen::Profile},
    };

    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f),
                        "ROCK  PAPER  SCISSORS");
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20, 0));
    ImGui::SameLine();

    for (const auto& item : items) {
        bool isActive = (currentScreen_ == item.screen);
        if (isActive) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f));
        }
        if (ImGui::Button(item.label, ImVec2(110, 32))) {
            currentScreen_ = item.screen;
        }
        if (isActive) {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
    }

    // Right-aligned save/load controls and status message.
    ImGui::SameLine(ImGui::GetWindowWidth() - 260);
    if (ImGui::Button("Save", ImVec2(70, 32))) {
        saveAllData();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", ImVec2(70, 32))) {
        loadAllData();
        refreshLeaderboardFromStats();
    }

    if (!statusMessage_.empty()) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f),
                            "%s", statusMessage_.c_str());
    }
}

// ---------------------------------------------------------------------
// Shared helper logic, used by multiple screens.
// ---------------------------------------------------------------------

void Application::startNewMatch() {
    std::string name = currentPlayerName();
    if (name.empty()) {
        name = "Player1";
    }
    engine_.startMatch(name, selectedMode_, selectedDifficulty_);
    history_.clearUndoStack();
    hasLastRoundResult_ = false;
    matchJustEnded_ = false;
    currentScreen_ = rps::Screen::Game;
}

void Application::handlePlayerMove(rps::Move move) {
    if (!engine_.isActive() || engine_.isMatchOver()) {
        return;
    }

    const rps::PlayerStats* stats = statsManager_.get(currentPlayerName());
    rps::RoundRecord round = engine_.playRound(move, stats);

    history_.pushRoundForUndo(round);
    statsManager_.recordRound(currentPlayerName(), round.playerMove, round.result);

    lastRoundResult_ = round;
    hasLastRoundResult_ = true;

    if (engine_.isMatchOver()) {
        finishCurrentMatch();
    }
}

void Application::handleUndoMove() {
    rps::RoundRecord undone;
    if (!history_.undoLastRound(undone)) {
        showStatusMessage("Nothing to undo");
        return;
    }
    engine_.undoRound(undone);
    hasLastRoundResult_ = false;
    matchJustEnded_ = false;
    showStatusMessage("Last move undone");
}

void Application::finishCurrentMatch() {
    rps::MatchRecord record = engine_.buildMatchRecord(history_.nextMatchId());
    history_.recordMatch(record);
    statsManager_.recordMatchResult(record.playerName, record.playerWonMatch);
    refreshLeaderboardFromStats();

    lastFinishedMatch_ = record;
    matchJustEnded_ = true;
}

void Application::refreshLeaderboardFromStats() {
    leaderboard_.clear();
    for (const auto& [name, stats] : statsManager_.all()) {
        leaderboard_.insertOrUpdate(name, stats.matchesWon, stats.matchesLost, stats.roundsDrawn);
    }
}

void Application::showStatusMessage(const std::string& message) {
    statusMessage_ = message;
    statusMessageTimer_ = 2.5;
}

void Application::saveAllData() {
    bool ok = dataManager_.saveAll(statsManager_, history_);
    showStatusMessage(ok ? "Progress saved" : "Save failed");
}

void Application::loadAllData() {
    dataManager_.loadAll(statsManager_, history_);
    showStatusMessage("Data loaded");
}

void Application::resetAllStatistics() {
    statsManager_.resetAll();
    history_.clearHistory();
    leaderboard_.clear();
    saveAllData();
    showStatusMessage("All statistics reset");
}

} // namespace rps::gui
