// HistoryScreen.cpp
// Displays the full match history (from GameHistory's vector) and a
// "recent activity" panel driven by GameHistory's internal queue.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"

namespace rps::gui {

namespace {
    ImVec4 outcomeColor(bool won) {
        return won ? ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f)
                   : ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f);
    }
}

void Application::renderHistoryScreen() {
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Match History");
    ImGui::Dummy(ImVec2(0, 8));

    const auto& allMatches = history_.getAllMatches();
    if (allMatches.empty()) {
        ImGui::TextWrapped("No matches recorded yet. Play a game to start building your history.");
        return;
    }

    // Recent matches (queue-backed) as a quick-glance strip along the top.
    ImGui::Text("Recent Activity (last %d matches)", 10);
    ImGui::BeginChild("RecentMatches", ImVec2(0, 90), true, ImGuiWindowFlags_HorizontalScrollbar);
    auto recent = history_.getRecentMatches();
    for (const auto& match : recent) {
        ImGui::BeginGroup();
        ImGui::TextColored(outcomeColor(match.playerWonMatch), match.playerWonMatch ? "WIN" : "LOSS");
        ImGui::Text("%d - %d", match.playerScore, match.aiScore);
        ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f),
                            "%s", modeToString(match.mode).c_str());
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(24, 0));
        ImGui::SameLine();
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0, 16));
    ImGui::Text("Full Log (%d matches)", static_cast<int>(allMatches.size()));

    ImGui::BeginChild("FullHistory", ImVec2(0, 0), true);
    if (ImGui::BeginTable("HistoryTable", 7,
                           ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
                           ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Player");
        ImGui::TableSetupColumn("Mode");
        ImGui::TableSetupColumn("Difficulty");
        ImGui::TableSetupColumn("Score");
        ImGui::TableSetupColumn("Result");
        ImGui::TableSetupColumn("Date");
        ImGui::TableHeadersRow();

        // Show newest matches first.
        for (auto it = allMatches.rbegin(); it != allMatches.rend(); ++it) {
            const auto& match = *it;
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", match.matchId);
            ImGui::TableNextColumn(); ImGui::Text("%s", match.playerName.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", modeToString(match.mode).c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", difficultyToString(match.difficulty).c_str());
            ImGui::TableNextColumn(); ImGui::Text("%d - %d", match.playerScore, match.aiScore);
            ImGui::TableNextColumn();
            ImGui::TextColored(outcomeColor(match.playerWonMatch), match.playerWonMatch ? "WIN" : "LOSS");
            ImGui::TableNextColumn(); ImGui::Text("%s", match.timestamp.c_str());
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

} // namespace rps::gui
