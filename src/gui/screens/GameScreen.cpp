// GameScreen.cpp
// The main gameplay screen: shows the current score, lets the player
// pick Rock/Paper/Scissors, displays the result of each round, and
// offers undo plus a summary popup once the match ends.
#include "gui/Application.h"
#include "gui/Theme.h"
#include "imgui.h"

namespace rps::gui {

namespace {
    ImVec4 resultColor(rps::RoundResult r) {
        switch (r) {
            case rps::RoundResult::Win:  return ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f);
            case rps::RoundResult::Lose: return ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f);
            case rps::RoundResult::Draw: return ImVec4(Colors::Warning[0], Colors::Warning[1], Colors::Warning[2], 1.0f);
        }
        return ImVec4(1, 1, 1, 1);
    }

    const char* resultHeadline(rps::RoundResult r) {
        switch (r) {
            case rps::RoundResult::Win:  return "You won the round!";
            case rps::RoundResult::Lose: return "The computer won the round.";
            case rps::RoundResult::Draw: return "It's a draw.";
        }
        return "";
    }
}

void Application::renderGameScreen() {
    if (!engine_.isActive()) {
        ImGui::TextWrapped("No match in progress. Head back to Home to configure and start a new game.");
        if (ImGui::Button("Go to Home", ImVec2(160, 36))) {
            currentScreen_ = rps::Screen::Home;
        }
        return;
    }

    ImGui::Text("Player: %s", engine_.getPlayerName().c_str());
    ImGui::SameLine(ImGui::GetWindowWidth() - 340);
    ImGui::Text("%s | %s", modeToString(engine_.getMode()).c_str(), difficultyToString(engine_.getDifficulty()).c_str());

    ImGui::Dummy(ImVec2(0, 10));

    // --- Scoreboard ---
    ImGui::BeginChild("Scoreboard", ImVec2(0, 90), true);
    float halfWidth = ImGui::GetWindowWidth() / 2.0f;

    ImGui::BeginGroup();
    ImGui::SetWindowFontScale(1.3f);
    ImGui::Text("You");
    ImGui::TextColored(ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f),
                        "%d", engine_.getPlayerScore());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndGroup();

    ImGui::SameLine(halfWidth - 40);
    ImGui::SetWindowFontScale(1.3f);
    ImGui::Text("vs");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SameLine(halfWidth + 20);
    ImGui::BeginGroup();
    ImGui::SetWindowFontScale(1.3f);
    ImGui::Text("Computer");
    ImGui::TextColored(ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f),
                        "%d", engine_.getAiScore());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndGroup();

    ImGui::EndChild();

    ImGui::Text("Round %d  |  First to %d round wins takes the match", engine_.getRoundNumber() + 1, engine_.getWinsNeeded());
    ImGui::Dummy(ImVec2(0, 16));

    // --- Move buttons ---
    bool matchOver = engine_.isMatchOver();
    ImGui::BeginDisabled(matchOver);

    float buttonWidth = 170.0f;
    if (ImGui::Button("Rock", ImVec2(buttonWidth, 90))) {
        handlePlayerMove(rps::Move::Rock);
    }
    ImGui::SameLine();
    if (ImGui::Button("Paper", ImVec2(buttonWidth, 90))) {
        handlePlayerMove(rps::Move::Paper);
    }
    ImGui::SameLine();
    if (ImGui::Button("Scissors", ImVec2(buttonWidth, 90))) {
        handlePlayerMove(rps::Move::Scissors);
    }

    ImGui::EndDisabled();

    ImGui::Dummy(ImVec2(0, 12));
    ImGui::BeginDisabled(!history_.canUndo() || matchOver);
    if (ImGui::Button("Undo Last Move", ImVec2(200, 36))) {
        handleUndoMove();
    }
    ImGui::EndDisabled();

    ImGui::Dummy(ImVec2(0, 16));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 8));

    // --- Last round result ---
    if (hasLastRoundResult_) {
        ImGui::TextColored(resultColor(lastRoundResult_.result), "%s", resultHeadline(lastRoundResult_.result));
        ImGui::Text("You played %s. Computer played %s.",
                    moveToString(lastRoundResult_.playerMove).c_str(),
                    moveToString(lastRoundResult_.aiMove).c_str());
    } else {
        ImGui::TextColored(ImVec4(Colors::Muted[0], Colors::Muted[1], Colors::Muted[2], 1.0f),
                            "Choose Rock, Paper, or Scissors to play your first round.");
    }

    ImGui::Dummy(ImVec2(0, 16));

    // --- Rounds played so far in this match ---
    ImGui::Text("Rounds this match:");
    ImGui::BeginChild("RoundsList", ImVec2(0, 150), true);
    if (ImGui::BeginTable("RoundsTable", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH)) {
        ImGui::TableSetupColumn("Round");
        ImGui::TableSetupColumn("You");
        ImGui::TableSetupColumn("Computer");
        ImGui::TableSetupColumn("Result");
        ImGui::TableHeadersRow();

        for (const auto& round : engine_.getCurrentRounds()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", round.roundNumber);
            ImGui::TableNextColumn(); ImGui::Text("%s", moveToString(round.playerMove).c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", moveToString(round.aiMove).c_str());
            ImGui::TableNextColumn();
            ImGui::TextColored(resultColor(round.result), "%s", resultToString(round.result).c_str());
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();

    // --- Match finished popup ---
    if (matchJustEnded_) {
        ImGui::OpenPopup("Match Result");
        matchJustEnded_ = false; // only trigger the popup once
    }

    ImGui::SetNextWindowSize(ImVec2(420, 0));
    if (ImGui::BeginPopupModal("Match Result", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        bool won = lastFinishedMatch_.playerWonMatch;
        ImGui::TextColored(won ? ImVec4(Colors::Success[0], Colors::Success[1], Colors::Success[2], 1.0f)
                                : ImVec4(Colors::Danger[0], Colors::Danger[1], Colors::Danger[2], 1.0f),
                            won ? "Victory!" : "Defeat");
        ImGui::Separator();
        ImGui::Text("Final Score  -  You: %d   Computer: %d",
                    lastFinishedMatch_.playerScore, lastFinishedMatch_.aiScore);
        ImGui::Text("Mode: %s   Difficulty: %s",
                    modeToString(lastFinishedMatch_.mode).c_str(),
                    difficultyToString(lastFinishedMatch_.difficulty).c_str());
        ImGui::Dummy(ImVec2(0, 12));

        if (ImGui::Button("Play Again", ImVec2(180, 40))) {
            ImGui::CloseCurrentPopup();
            startNewMatch();
        }
        ImGui::SameLine();
        if (ImGui::Button("Back to Home", ImVec2(180, 40))) {
            ImGui::CloseCurrentPopup();
            currentScreen_ = rps::Screen::Home;
        }
        ImGui::EndPopup();
    }
}

} // namespace rps::gui
