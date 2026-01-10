//
// Created by okker on 10/01/2026.
//

#pragma once
#include <array>

#include "editor_panel_interface.h"


namespace editor
{
    class StatsPanel final : public IEditorPanel
    {
    public:
        StatsPanel();

        void Render() override;
        void Update(float deltaTime);

        [[nodiscard]] const char *GetName() const override { return "Stats"; }
        [[nodiscard]] bool IsOpen() const override { return m_open; }

        void SetOpen(const bool open) override { m_open = open; };

    private:
        bool m_open{true};

        static constexpr size_t HISTORY_SIZE = 100;

        std::array<float, HISTORY_SIZE> m_frameTimeHistory{};
        size_t m_historyIndex{0};

        float m_averageFPS{0.f};
        float m_averageFrameTime{0.f};
        float m_minFrameTime{0.f};
        float m_maxFrameTime{0.f};
    };
}
