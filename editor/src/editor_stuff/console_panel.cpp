//
// Created by okker on 10/01/2026.
//

#include "console_panel.h"

#include "imgui.h"

namespace editor {
    Console::Console()
    {
        skal::Log::SetCallback([this](skal::LogLevel level, const std::string& msg)
        {
            AddLog(level, msg);
        });
    }

    Console::~Console()
    {
        skal::Log::SetCallback(nullptr);
    }


    void Console::Render()
    {
        if (!m_open) return;

        ImGui::Begin(GetName(), &m_open);

        if (ImGui::Button("Clear")) {
            Clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &m_autoScroll);
        ImGui::SameLine();
        ImGui::Checkbox("Info", &m_showInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warning", &m_showWarning);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &m_showError);

        ImGui::Separator();

        // Log display
        ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& entry : m_logs) {
            if (!ShouldShow(entry.level)) continue;

            ImVec4 color = GetColorForLevel(entry.level);
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            std::string logLine = "[" + GetLevelString(entry.level) + "] " + entry.message;
            ImGui::TextUnformatted(logLine.c_str());
            ImGui::PopStyleColor();
        }

        if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();

    }


    void Console::AddLog(const skal::LogLevel level, const std::string &message)
    {
        m_logs.push_back({level, message});
    }

    void Console::Clear()
    {
        m_logs.clear();
    }

    ImVec4 Console::GetColorForLevel(skal::LogLevel level)
    {
        switch (level) {
            case skal::LogLevel::Info: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            case skal::LogLevel::Warning: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            case skal::LogLevel::Error: return ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            case skal::LogLevel::Critical: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    bool Console::ShouldShow(skal::LogLevel level) const
    {
        switch (level) {
            case skal::LogLevel::Info: return m_showInfo;
            case skal::LogLevel::Warning: return m_showWarning;
            case skal::LogLevel::Error:
            case skal::LogLevel::Critical: return m_showError;
        }
        return true;
    }

    std::string Console::GetLevelString(skal::LogLevel level)
    {
        switch (level) {
            case skal::LogLevel::Info: return "INFO";
            case skal::LogLevel::Warning: return "WARN";
            case skal::LogLevel::Error: return "ERROR";
            case skal::LogLevel::Critical: return "CRITICAL";
        }
        return "UNKNOWN";
    }
} // editor