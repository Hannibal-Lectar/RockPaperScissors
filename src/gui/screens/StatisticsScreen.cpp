// StatisticsScreen.cpp
// A dashboard summarizing the current player's performance, plus a
// reset-statistics control.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"
#include <cstdio>

namespace rps::gui {

void Application::renderStatisticsScreen() {
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Statistics Dashboard");
    ImGui::Dummy(ImVec2(0, 8));

    std::string name = currentPlayerName();
    const rps::PlayerStats* stats = statsManager_.get(name);

    if (stats == nullptr || stats->matchesPlayed == 0) {
        ImGui::TextWrapped("No statistics yet for \"%s\". Play a match to populate this dashboard.", name.c_str());
        return;
    }

    // --- Top summary cards ---
    auto card = [](const char* title, const std::string& value, const float* color) {
        ImGui::BeginChild(title, ImVec2(220, 100), true);
        ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f), "%s", title);
        ImGui::SetWindowFontScale(1.6f);
        ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.0f), "%s", value.c_str());
        ImGui::SetWindowFontScale(1.0f);
        ImGui::EndChild();
        ImGui::SameLine();
    };

    card("Matches Played", std::to_string(stats->matchesPlayed), Colors::Accent);
    card("Matches Won",    std::to_string(stats->matchesWon),    Colors::Success);
    card("Win Rate",       [&]{ char b[16]; snprintf(b, sizeof(b), "%.1f%%", stats->winRate()); return std::string(b); }(), Colors::Warning);
    card("Best Streak",    std::to_string(stats->bestStreak),    Colors::Danger);
    ImGui::NewLine();

    ImGui::Dummy(ImVec2(0, 20));

    // --- Round breakdown ---
    ImGui::BeginChild("RoundBreakdown", ImVec2(360, 220), true);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Round Breakdown");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));
    ImGui::Text("Total Rounds Played: %d", stats->roundsPlayed);
    ImGui::TextColored(ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f),
                        "Rounds Won:   %d", stats->roundsWon);
    ImGui::TextColored(ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f),
                        "Rounds Lost:  %d", stats->roundsLost);
    ImGui::TextColored(ImVec4(Colors::Warning[0], Colors::Warning[1], Colors::Warning[2], 1.0f),
                        "Rounds Drawn: %d", stats->roundsDrawn);

    if (stats->roundsPlayed > 0) {
        ImGui::Dummy(ImVec2(0, 10));
        float winFrac = static_cast<float>(stats->roundsWon) / static_cast<float>(stats->roundsPlayed);
        ImGui::Text("Round Win Rate");
        ImGui::ProgressBar(winFrac, ImVec2(-1, 20));
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20, 0));
    ImGui::SameLine();

    // --- Move usage breakdown ---
    ImGui::BeginChild("MoveBreakdown", ImVec2(360, 220), true);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Move Usage");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    int totalMoves = stats->rockCount + stats->paperCount + stats->scissorsCount;
    auto moveBar = [&](const char* label, int count) {
        float frac = totalMoves == 0 ? 0.0f : static_cast<float>(count) / static_cast<float>(totalMoves);
        ImGui::Text("%s (%d)", label, count);
        ImGui::ProgressBar(frac, ImVec2(-1, 18));
    };
    moveBar("Rock", stats->rockCount);
    moveBar("Paper", stats->paperCount);
    moveBar("Scissors", stats->scissorsCount);

    ImGui::Dummy(ImVec2(0, 8));
    ImGui::Text("Favorite Move: %s", moveToString(stats->favoriteMove()).c_str());
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0, 24));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f));
    if (ImGui::Button("Reset All Statistics", ImVec2(220, 40))) {
        ImGui::OpenPopup("Confirm Reset");
    }
    ImGui::PopStyleColor();

    if (ImGui::BeginPopupModal("Confirm Reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("This will permanently erase ALL players' statistics,");
        ImGui::Text("match history, and leaderboard standings.");
        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Yes, reset everything", ImVec2(200, 36))) {
            resetAllStatistics();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 36))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

} // namespace rps::gui
