//
// Created by okker on 10/01/2026.
//

#include "stats_panel.h"

#include "imgui.h"

editor::StatsPanel::StatsPanel() {}
void editor::StatsPanel::Render()
{
    if (!m_open) return;

    ImGui::Begin(GetName(), &m_open);

    ImGui::Text("FPS: %.1f", m_averageFPS);
    ImGui::Text("Frame Time: %.2f ms", m_averageFrameTime);
    ImGui::Text("Min: %.2f ms | Max: %.2f ms", m_minFrameTime, m_maxFrameTime);

    ImVec2 plotSize = ImGui::GetContentRegionAvail();
    plotSize.y = 80;

    ImGui::PlotLines("##", m_frameTimeHistory.data(), HISTORY_SIZE,
                     m_historyIndex, "Frame Time (ms)",
                     0.f, m_maxFrameTime * 1.2f, plotSize);
    ImGui::End();
}

void editor::StatsPanel::Update(float deltaTime)
{
    m_frameTimeHistory[m_historyIndex] = deltaTime * 1000.f;  // ms
    m_historyIndex = (m_historyIndex + 1) % HISTORY_SIZE;

    float sum = 0.f;
    m_minFrameTime = m_frameTimeHistory[0];
    m_maxFrameTime = m_frameTimeHistory[0];

    for (float time : m_frameTimeHistory) {
        sum += time;
        m_minFrameTime = std::min(m_minFrameTime, time);
        m_maxFrameTime = std::max(m_maxFrameTime, time);
    }

    m_averageFrameTime = sum / HISTORY_SIZE;
    m_averageFPS = 1000.f / m_averageFrameTime;
}
