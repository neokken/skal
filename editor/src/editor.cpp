//
// Created by okker on 30/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "editor.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "editor_stuff/console_panel.h"
#include "editor_stuff/imgui_demo_panel.h"
#include "editor_stuff/viewport.h"
#include "skal/engine.h"
#include "skal/file_io/file_io.h"
#include "skal/platform/sdl3/device/device_sdl3.h"
#include "skal/resource/resource_indexer.h"
#include "skal/resource/resource_manager.h"
#include "skal/resource/types/mesh_resource.h"
#include "skal/util/log.h"


namespace skal
{
    class Mesh;
}

namespace editor
{
    void Editor::InitializeEditor()
    {
        skal::Log::Initialize();

        m_panels.push_back(std::make_unique<Console>());

        skal::Engine.Initialize();

        m_imguiContext = std::make_unique<ImGuiDebugUI>(skal::Engine.Device().GetNativeWindowHandle());
        skal::Engine.SetDebugUI(m_imguiContext.get());



        auto viewport = std::make_unique<Viewport>();
        // ReSharper disable once CppDFALocalValueEscapesFunction
        m_viewport = viewport.get();
        m_panels.push_back(std::move(viewport));


        auto stats = std::make_unique<StatsPanel>();
        // ReSharper disable once CppDFALocalValueEscapesFunction
        m_statsPanel = stats.get();
        m_panels.push_back(std::move(stats));


        m_panels.push_back(std::make_unique<ImGuiDemoPanel>());
    }

    void Editor::Shutdown()
    {
        m_panels.clear();
        skal::Engine.Shutdown();
    }

    bool Editor::LoadDefaultLayout()
    {
        const std::string defaultLayoutPath = m_projectPath + "/editor/editor_layout_default.ini";
        if (skal::Engine.FileIO().Exists(defaultLayoutPath))
        {
            ImGui::LoadIniSettingsFromDisk(defaultLayoutPath.c_str());
            return true;
        }

        return false;
    }


    void Editor::RenderToolbar()
    {
        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) { skal::Engine.RequestClose(); }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Load default layout"))
            {
                if (!LoadDefaultLayout())
                {
                    skal::Log::Error("Failed to load default layout.");
                }
            }
            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }

    void Editor::RenderEditorPanels() {}


    void Editor::Run(const std::string &projectPath)
    {
        InitializeEditor();

        if (!OpenProject(projectPath))
        {
            skal::Log::Error("Editor::main - Opening project failed");
            skal::Engine.Shutdown();
            return;
        }



        auto mesh = skal::Engine.ResourceManager().Load<skal::Mesh>(skal::ResourceUUID::from_string("981f34b1-cc32-4bac-b094-6a2bbfb06ed3"));

        float time{0};

        while (skal::Engine.ShouldRun())
        {

            skal::Engine.PreUpdate();
            m_imguiContext->BeginFrame();
            ImGui::DockSpaceOverViewport();

            //if (editor.GetPlayState() == editor::EditorContext::PlayState::Playing)
            //{
            //    skal::Engine.GameUpdate();
            //}

            skal::Engine.PostUpdate();

            m_statsPanel->Update(skal::Engine.GetRawDeltaTime());


            skal::Engine.RenderScene(m_viewport->GetViewportFrameData());

            time += skal::Engine.GetDeltaTime();
            if (mesh.IsValid())
            {
                skal::DrawCommand drawCommand;
                drawCommand.mesh = mesh.Get()->GetRenderHandle();

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -2.f));
                glm::quat angle = glm::angleAxis(glm::radians(time*100), glm::vec3(0.f, 1.f, 0.f));
                angle = angle * glm::angleAxis(glm::radians(time * 50.f), glm::vec3(1, 0.f, 0.f));
                transform = transform * glm::mat4_cast(angle);
                drawCommand.transform = transform;

                skal::Engine.Renderer().Submit(drawCommand);
            }


            for (const auto& panel : m_panels) panel->Render();

            RenderToolbar();
            skal::Engine.PresentFrame();
        }

                mesh = {};

        Shutdown();
    }

    bool Editor::OpenProject(const std::string &path)
    {
        m_projectPath = path;
        const std::string projectFilePath = m_projectPath + "/project.skal";

        if (!skal::Engine.FileIO().Exists(projectFilePath))
        {
            skal::Log::Error("Editor::OpenProject No project.skal found at {}", projectFilePath);
            return false;
        }


        // Index assets

        skal::Engine.ResourceIndexer().IndexDirectory(m_projectPath + "/assets");


        // Load project file
        const auto json = nlohmann::json::parse(skal::Engine.FileIO().ReadTextFile(projectFilePath));
        skal::Project project = skal::DeserializeProject(json);

        skal::Engine.LoadProject(project);

        skal::Log::Info("Opened project: {}", project.name);


        // Layout

        m_panelLayoutPath = m_projectPath + "/.editor/editor_layout.ini";
        ImGui::GetIO().IniFilename = m_panelLayoutPath.c_str();

        if (skal::Engine.FileIO().Exists(m_panelLayoutPath))
        {
            ImGui::LoadIniSettingsFromDisk(m_panelLayoutPath.c_str());
        }
        else
        {
            skal::Engine.FileIO().WriteTextFile(m_panelLayoutPath.c_str(), "");
            if (!LoadDefaultLayout())
            {
                skal::Log::Error("Editor::OpenProject - Could not load default layout, since no default layout file, doesnt exist");
            }
        }



        return true;
    }


} // editor
