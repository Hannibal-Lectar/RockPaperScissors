// ProfileScreen.cpp
// Lets the user browse any player that has recorded statistics, view
// their detailed profile, switch the active player, or reset just
// that one player's stats.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"

#include <cstdio>
#include <vector>
#include <algorithm>

namespace rps::gui {

void Application::renderProfileScreen() {
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Player Profiles");
    ImGui::Dummy(ImVec2(0, 8));

    const auto& allStats = statsManager_.all();
    if (allStats.empty()) {
        ImGui::TextWrapped("No player profiles yet. Play a match to create one.");
        return;
    }

    // Sorted player name list for stable, predictable ordering.
    std::vector<std::string> names;
    names.reserve(allStats.size());
    for (const auto& [name, stats] : allStats) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    if (profileSelectedPlayer_.empty() || allStats.find(profileSelectedPlayer_) == allStats.end()) {
        profileSelectedPlayer_ = names.front();
    }

    // --- Player list panel ---
    ImGui::BeginChild("PlayerList", ImVec2(240, 0), true);
    ImGui::Text("Players");
    ImGui::Separator();
    for (const auto& name : names) {
        bool selected = (name == profileSelectedPlayer_);
        if (ImGui::Selectable(name.c_str(), selected)) {
            profileSelectedPlayer_ = name;
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // --- Profile detail panel ---
    ImGui::BeginChild("ProfileDetail", ImVec2(0, 0), true);
    const rps::PlayerStats& stats = allStats.at(profileSelectedPlayer_);

    ImGui::SetWindowFontScale(1.4f);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f),
                        "%s", stats.name.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 8));

    ImGui::Columns(2, "ProfileColumns", false);
    ImGui::Text("Matches Played"); ImGui::NextColumn(); ImGui::Text("%d", stats.matchesPlayed); ImGui::NextColumn();
    ImGui::Text("Matches Won");    ImGui::NextColumn(); ImGui::Text("%d", stats.matchesWon);    ImGui::NextColumn();
    ImGui::Text("Matches Lost");   ImGui::NextColumn(); ImGui::Text("%d", stats.matchesLost);   ImGui::NextColumn();
    ImGui::Text("Win Rate");       ImGui::NextColumn(); ImGui::Text("%.1f%%", stats.winRate());  ImGui::NextColumn();
    ImGui::Text("Current Streak"); ImGui::NextColumn(); ImGui::Text("%d", stats.currentStreak);  ImGui::NextColumn();
    ImGui::Text("Best Streak");    ImGui::NextColumn(); ImGui::Text("%d", stats.bestStreak);     ImGui::NextColumn();
    ImGui::Text("Favorite Move");  ImGui::NextColumn(); ImGui::Text("%s", moveToString(stats.favoriteMove()).c_str()); ImGui::NextColumn();
    ImGui::Columns(1);

    ImGui::Dummy(ImVec2(0, 20));

    if (ImGui::Button("Play as this Player", ImVec2(220, 40))) {
        // Copy the selected profile's name into the active player-name
        // buffer so the next match started uses this profile.
        std::snprintf(playerNameBuffer_, sizeof(playerNameBuffer_), "%s", stats.name.c_str());
        showStatusMessage("Active player set to " + stats.name);
        currentScreen_ = rps::Screen::Home;
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f));
    if (ImGui::Button("Reset This Player", ImVec2(220, 40))) {
        ImGui::OpenPopup("Confirm Player Reset");
    }
    ImGui::PopStyleColor();

    if (ImGui::BeginPopupModal("Confirm Player Reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Reset all statistics for \"%s\"?", stats.name.c_str());
        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Yes, reset", ImVec2(140, 36))) {
            statsManager_.resetPlayer(profileSelectedPlayer_);
            refreshLeaderboardFromStats();
            saveAllData();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 36))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::EndChild();
}

} // namespace rps::gui
