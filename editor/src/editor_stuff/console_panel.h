//
// Created by okker on 10/01/2026.
//

#pragma once
#include "editor_panel_interface.h"
#include "imgui.h"
#include "skal/util/log.h"

namespace editor
{
    class Console final : public IEditorPanel
    {
    public:
        Console();
        ~Console() override;

        void Render() override;
        [[nodiscard]] const char * GetName() const override {return "Console";}
        [[nodiscard]] bool IsOpen() const override {return m_open;}
        void SetOpen(const bool open) override {m_open = open;};

        void AddLog(skal::LogLevel level, const std::string& message);
        void Clear();
    private:
        bool ShouldShow(skal::LogLevel level) const;

        static std::string GetLevelString(skal::LogLevel level);


        static ImVec4 GetColorForLevel(skal::LogLevel level);


        private:
        bool m_open{true};

        struct LogEntry {
            skal::LogLevel level;
            std::string message;
            std::string timestamp;
        };

        std::vector<LogEntry> m_logs;
        bool m_autoScroll{true};
        bool m_showInfo{true};
        bool m_showWarning{true};
        bool m_showError{true};
    };
} // editor