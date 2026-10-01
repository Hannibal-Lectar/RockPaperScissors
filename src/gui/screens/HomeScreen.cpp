// HomeScreen.cpp
// The landing screen: lets the player enter their name, pick a match
// mode and difficulty, and start a new game.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"

namespace rps::gui {

namespace {
    // Save files use '|' and ';' as field separators, so we block those
    // characters from player names to keep the on-disk format valid.
    int filterPlayerNameChars(ImGuiInputTextCallbackData* data) {
        if (data->EventChar == '|' || data->EventChar == ';') {
            return 1; // reject the character
        }
        return 0;
    }
}

void Application::renderHomeScreen() {
    ImGui::BeginChild("HomeContent", ImVec2(0, 0), false);

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::PushFont(nullptr); // keep default font; reserved for future custom fonts
    ImGui::SetWindowFontScale(1.6f);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f),
                        "Welcome to Rock, Paper, Scissors");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopFont();

    ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f),
                        "Play against a computer opponent, track your stats, and climb the leaderboard.");
    ImGui::Dummy(ImVec2(0, 20));

    ImGui::BeginChild("SetupPanel", ImVec2(520, 340), true);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Match Setup");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 8));

    ImGui::Text("Player Name");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##PlayerName", playerNameBuffer_, IM_ARRAYSIZE(playerNameBuffer_),
                      ImGuiInputTextFlags_CallbackCharFilter, filterPlayerNameChars);

    ImGui::Dummy(ImVec2(0, 14));
    ImGui::Text("Match Length");
    ImGui::Spacing();
    struct ModeOption { const char* label; rps::MatchMode mode; };
    static const ModeOption modeOptions[] = {
        {"Best of 3", rps::MatchMode::BestOf3},
        {"Best of 5", rps::MatchMode::BestOf5},
        {"Best of 7", rps::MatchMode::BestOf7},
    };
    for (const auto& option : modeOptions) {
        bool selected = (selectedMode_ == option.mode);
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f));
        }
        if (ImGui::Button(option.label, ImVec2(150, 34))) {
            selectedMode_ = option.mode;
        }
        if (selected) {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
    }
    ImGui::NewLine();

    ImGui::Dummy(ImVec2(0, 14));
    ImGui::Text("Difficulty");
    ImGui::Spacing();
    struct DiffOption { const char* label; rps::Difficulty diff; const float* color; };
    static const DiffOption diffOptions[] = {
        {"Easy",   rps::Difficulty::Easy,   Colors::Success},
        {"Medium", rps::Difficulty::Medium, Colors::Warning},
        {"Hard",   rps::Difficulty::Hard,   Colors::Danger},
    };
    for (const auto& option : diffOptions) {
        bool selected = (selectedDifficulty_ == option.diff);
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(option.color[0], option.color[1], option.color[2], 1.0f));
        }
        if (ImGui::Button(option.label, ImVec2(150, 34))) {
            selectedDifficulty_ = option.diff;
        }
        if (selected) {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
    }
    ImGui::NewLine();

    ImGui::Dummy(ImVec2(0, 22));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f));
    if (ImGui::Button("Start Game", ImVec2(-1, 48))) {
        startNewMatch();
    }
    ImGui::PopStyleColor();

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20, 0));
    ImGui::SameLine();

    ImGui::BeginChild("QuickStatsPanel", ImVec2(360, 340), true);
    ImGui::TextColored(ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], 1.0f), "Your Snapshot");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 8));

    const rps::PlayerStats* stats = statsManager_.get(currentPlayerName());
    if (stats == nullptr || stats->matchesPlayed == 0) {
        ImGui::TextWrapped("No games played yet for \"%s\". Start your first match to begin building your stats!",
                            currentPlayerName().c_str());
    } else {
        ImGui::Text("Matches Played: %d", stats->matchesPlayed);
        ImGui::Text("Matches Won:    %d", stats->matchesWon);
        ImGui::Text("Win Rate:       %.1f%%", stats->winRate());
        ImGui::Text("Current Streak: %d", stats->currentStreak);
        ImGui::Text("Best Streak:    %d", stats->bestStreak);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("Favorite Move:  %s", moveToString(stats->favoriteMove()).c_str());
    }

    ImGui::Dummy(ImVec2(0, 16));
    ImGui::Separator();
    ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f), "Tip");
    ImGui::TextWrapped("On Hard difficulty, the computer studies your move history, "
                        "so mixing up your strategy pays off.");

    ImGui::EndChild();

    ImGui::EndChild();
}

} // namespace rps::gui
