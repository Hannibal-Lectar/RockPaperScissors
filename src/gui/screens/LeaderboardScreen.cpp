// LeaderboardScreen.cpp
// Renders the ranking produced by the custom linked-list Leaderboard.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"

namespace rps::gui {

void Application::renderLeaderboardScreen() {
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Leaderboard");
    ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f),
                        "Ranked by total match wins across all players.");
    ImGui::Dummy(ImVec2(0, 10));

    auto ranked = leaderboard_.toSortedVector();
    if (ranked.empty()) {
        ImGui::TextWrapped("No players on the leaderboard yet. Win a match to claim the top spot!");
        return;
    }

    ImGui::BeginChild("LeaderboardTable", ImVec2(0, 0), true);
    if (ImGui::BeginTable("LeaderboardTableInner", 5,
                           ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Rank", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Player");
        ImGui::TableSetupColumn("Wins");
        ImGui::TableSetupColumn("Losses");
        ImGui::TableSetupColumn("Win Rate");
        ImGui::TableHeadersRow();

        int rank = 1;
        for (const auto& entry : ranked) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            if (rank == 1) {
                ImGui::TextColored(ImVec4(Colors::Warning[0], Colors::Warning[1], Colors::Warning[2], 1.0f), "#1");
            } else {
                ImGui::Text("#%d", rank);
            }

            ImGui::TableNextColumn();
            ImGui::Text("%s", entry.playerName.c_str());
            if (entry.playerName == currentPlayerName()) {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "(you)");
            }

            ImGui::TableNextColumn();
            ImGui::TextColored(ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f), "%d", entry.wins);

            ImGui::TableNextColumn();
            ImGui::TextColored(ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f), "%d", entry.losses);

            ImGui::TableNextColumn();
            ImGui::Text("%.1f%%", entry.winRate);

            rank++;
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

} // namespace rps::gui
