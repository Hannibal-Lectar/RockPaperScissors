// Theme.h
// Applies a modern, polished dark theme to Dear ImGui. Kept separate
// from Application so the visual styling can be tweaked independently
// of the application/window logic.
#pragma once

struct ImGuiStyle;

namespace rps::gui {

// Applies custom colors, rounding, spacing and font scaling to give the
// application a modern dark look rather than ImGui's default styling.
void applyDarkTheme();

// Accent colors reused across multiple screens so the palette stays
// consistent (e.g. green for wins, red for losses, gold for the
// leaderboard's top rank).
namespace Colors {
    extern const float Accent[4];
    extern const float AccentHover[4];
    extern const float Success[4];
    extern const float Danger[4];
    extern const float Warning[4];
    extern const float Muted[4];
}

} // namespace rps::gui
