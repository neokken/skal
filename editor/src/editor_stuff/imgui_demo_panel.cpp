//
// Created by okker on 10/01/2026.
//

#include "imgui_demo_panel.h"

#include "imgui.h"

void editor::ImGuiDemoPanel::Render()
{
    if (m_open)
        ImGui::ShowDemoWindow(&m_open);
}
