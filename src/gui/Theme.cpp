// Theme.cpp
#include "gui/Theme.h"
#include "imgui.h"

namespace rps::gui {

namespace Colors {
    const float Accent[4]      = {0.35f, 0.55f, 0.95f, 1.00f}; // blue
    const float AccentHover[4] = {0.45f, 0.65f, 1.00f, 1.00f};
    const float Success[4]     = {0.30f, 0.80f, 0.45f, 1.00f}; // green
    const float Danger[4]      = {0.90f, 0.35f, 0.35f, 1.00f}; // red
    const float Warning[4]     = {0.95f, 0.75f, 0.25f, 1.00f}; // gold
    const float Muted[4]       = {0.55f, 0.58f, 0.65f, 1.00f}; // gray
}

void applyDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- Layout / shape ---
    style.WindowRounding    = 8.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 6.0f;
    style.PopupRounding     = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding      = 6.0f;
    style.TabRounding       = 6.0f;

    style.WindowPadding  = ImVec2(14, 14);
    style.FramePadding   = ImVec2(10, 6);
    style.ItemSpacing     = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing   = 20.0f;
    style.ScrollbarSize   = 14.0f;
    style.GrabMinSize     = 10.0f;

    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize  = 1.0f;
    style.PopupBorderSize  = 1.0f;

    // --- Palette: a deep charcoal dark theme with a cool blue accent ---
    const ImVec4 bgBase       = ImVec4(0.09f, 0.10f, 0.13f, 1.00f);
    const ImVec4 bgPanel      = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
    const ImVec4 bgPanelLight = ImVec4(0.17f, 0.18f, 0.23f, 1.00f);
    const ImVec4 border       = ImVec4(0.24f, 0.25f, 0.31f, 0.60f);
    const ImVec4 text         = ImVec4(0.92f, 0.93f, 0.95f, 1.00f);
    const ImVec4 textDisabled = ImVec4(0.50f, 0.52f, 0.58f, 1.00f);
    const ImVec4 accent       = ImVec4(Colors::Accent[0], Colors::Accent[1], Colors::Accent[2], Colors::Accent[3]);
    const ImVec4 accentHover  = ImVec4(Colors::AccentHover[0], Colors::AccentHover[1], Colors::AccentHover[2], Colors::AccentHover[3]);
    const ImVec4 accentActive = ImVec4(0.28f, 0.46f, 0.85f, 1.00f);

    colors[ImGuiCol_Text]                  = text;
    colors[ImGuiCol_TextDisabled]          = textDisabled;
    colors[ImGuiCol_WindowBg]              = bgBase;
    colors[ImGuiCol_ChildBg]               = bgPanel;
    colors[ImGuiCol_PopupBg]               = bgPanelLight;
    colors[ImGuiCol_Border]                = border;
    colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

    colors[ImGuiCol_FrameBg]               = bgPanelLight;
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.22f, 0.24f, 0.30f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.28f, 0.36f, 1.00f);

    colors[ImGuiCol_TitleBg]               = bgPanel;
    colors[ImGuiCol_TitleBgActive]         = bgPanel;
    colors[ImGuiCol_TitleBgCollapsed]      = bgPanel;
    colors[ImGuiCol_MenuBarBg]             = bgPanel;

    colors[ImGuiCol_ScrollbarBg]           = bgBase;
    colors[ImGuiCol_ScrollbarGrab]         = bgPanelLight;
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.30f, 0.32f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]   = accent;

    colors[ImGuiCol_CheckMark]             = accent;
    colors[ImGuiCol_SliderGrab]            = accent;
    colors[ImGuiCol_SliderGrabActive]      = accentActive;

    colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.22f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = accentHover;
    colors[ImGuiCol_ButtonActive]          = accentActive;

    colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.22f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.29f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderActive]          = accentActive;

    colors[ImGuiCol_Separator]             = border;
    colors[ImGuiCol_SeparatorHovered]      = accent;
    colors[ImGuiCol_SeparatorActive]       = accentActive;

    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.29f, 0.38f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered]     = accentHover;
    colors[ImGuiCol_ResizeGripActive]      = accentActive;

    colors[ImGuiCol_Tab]                   = bgPanel;
    colors[ImGuiCol_TabHovered]            = accentHover;
    colors[ImGuiCol_TabActive]             = accentActive;
    colors[ImGuiCol_TabUnfocused]          = bgPanel;
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.20f, 0.22f, 0.28f, 1.00f);

    colors[ImGuiCol_PlotLines]             = accent;
    colors[ImGuiCol_PlotLinesHovered]      = accentHover;
    colors[ImGuiCol_PlotHistogram]         = accent;
    colors[ImGuiCol_PlotHistogramHovered]  = accentHover;

    colors[ImGuiCol_TableHeaderBg]         = bgPanelLight;
    colors[ImGuiCol_TableBorderStrong]     = border;
    colors[ImGuiCol_TableBorderLight]      = ImVec4(0.20f, 0.21f, 0.26f, 0.50f);
    colors[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1, 1, 1, 0.02f);

    colors[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.35f);
    colors[ImGuiCol_DragDropTarget]        = accent;
    colors[ImGuiCol_NavHighlight]          = accent;
}

} // namespace rps::gui
