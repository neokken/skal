//
// Created by okker on 30/12/2025.
//

#pragma once
#include <memory>
#include <string>

#include "editor_stuff/stats_panel.h"
#include "editor_stuff/viewport.h"
#include "imgui/imgui_debug_ui.h"
#include "skal/renderering/renderer.h"



namespace editor
{

    class Editor
    {
    public:
        Editor() = default;

        void Run(const std::string& projectPath);


    private:
        bool OpenProject(const std::string& path);
        void InitializeEditor();
        void Shutdown();

        bool LoadDefaultLayout();

        void RenderToolbar();
        void RenderEditorPanels();

    private:
        std::string m_projectPath{};
        std::unique_ptr<editor::ImGuiDebugUI> m_imguiContext;


        // tools
        std::string m_panelLayoutPath{};

        std::vector<std::unique_ptr<IEditorPanel>> m_panels;

        Viewport* m_viewport{nullptr};   // non owned, just references, so for easier lookup.
        StatsPanel* m_statsPanel{nullptr};

    };

} // editor