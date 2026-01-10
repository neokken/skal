//
// Created by okker on 10/01/2026.
//

#pragma once
#include "editor_panel_interface.h"


namespace editor
{
    class ImGuiDemoPanel final : public IEditorPanel
    {
    public:
        void Render() override;

        const char* GetName() const override {return "ImguiDemo";}

        [[nodiscard]] bool IsOpen() const override {return m_open;}

        void SetOpen(const bool open) override { m_open = open; }

    private:
        bool m_open{true};
    };
}
