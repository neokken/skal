//
// Created by okker on 20/12/2025.
//

#include <iostream>
#include <skal/engine.h>

#include "editor_context.h"
#include "imgui.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "imgui/imgui_debug_ui.h"
#include "skal/uuid_types.h"
#include "skal/input/input.h"
#include "skal/platform/sdl3/device/device_sdl3.h"
#include "skal/resource/resource_manager.h"
#include "skal/resource/types/mesh_resource.h"
#include "skal/resource/types/texture_resource.h"
#include "skal/util/log.h"
#include "utils/file_dialogue.h"
#include "utils/project_dialog.h"

int main(const int argc, char **argv)
{
    skal::Engine.Initialize();

    editor::EditorContext editor;
    std::string projectPath;

    if (argc > 1)
    {
        // Command line: editor.exe "C:/Projects/MyGame"    // full path, or it's appended to the working directory
        projectPath = argv[1];
    } else
    {
        /// Show dialog with options
        const auto choice = editor::ShowProjectStartDialog();

        if (choice == editor::ProjectAction::Open)
        {
            projectPath = editor::FileDialogue::OpenFolder();
            if (projectPath.empty())
            {
                // user cancelled
                skal::Engine.Shutdown();
                return 0;
            }
        } else if (choice == editor::ProjectAction::CreateNew)
        {
            const std::string projectName = editor::PromptProjectName();
            const std::string folderPath = editor::FileDialogue::OpenFolder();

            if (editor::EditorContext::CreateProject(folderPath, projectName))
            {
                projectPath = folderPath + "/" + projectName;
            }
        } else
        {
            // user cancelled
            skal::Engine.Shutdown();
            return 0;
        }
    }


    if (!editor.OpenProject(projectPath))
    {
        skal::Log::Error("Editor::main - Opening project failed");
        skal::Engine.Shutdown();
        return 1;
    }

    editor::ImGuiDebugUI ui = editor::ImGuiDebugUI(skal::Engine.Device().GetNativeWindowHandle());
    skal::Engine.SetDebugUI(&ui);


    auto buffer = skal::Engine.Renderer().CreateFrameBuffer(1024, 768);
    auto mesh = skal::Engine.ResourceManager().Load<skal::Mesh>(
        skal::ResourceUUID::from_string("981f34b1-cc32-4bac-b094-6a2bbfb06ed3"));

    float time{0};


    while (skal::Engine.ShouldRun())
    {
        skal::Engine.PreUpdate();
        ui.BeginFrame();

        if (editor.GetPlayState() == editor::EditorContext::PlayState::Playing)
        {
            skal::Engine.GameUpdate();
        }

        skal::Engine.PostUpdate();

        ImGui::Begin("Viewport");
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();

        if (viewport_size.x > 0 && viewport_size.y > 0)
        {
            uint32_t texture_id = skal::Engine.Renderer().GetTextureId(buffer);
            ImGui::Image(
                reinterpret_cast<void *>(static_cast<uintptr_t>(texture_id)),
                viewport_size,
                ImVec2(0, 1), // UV top-left (flipped for OpenGL)
                ImVec2(1, 0) // UV bottom-right
            );
        }

        ImGui::End();

        skal::FrameData frameData;
        if (viewport_size.x > 0 && viewport_size.y > 0)
        {

            frameData.frame_buffer = buffer;
            frameData.width = static_cast<uint16_t>(viewport_size.x);
            frameData.height = static_cast<uint16_t>(viewport_size.y);
            frameData.clear_color = glm::vec4(skal::colors::lime, 1.f);
            skal::Engine.RenderScene(&frameData);
        }
        else
        {
            skal::Engine.RenderScene();
        }

        time += 1 / 60.f;
        if (mesh.IsValid())
        {
            skal::DrawCommand drawCommand;
            drawCommand.mesh = mesh.Get()->GetRenderHandle();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -2.f));
            glm::quat angle = glm::angleAxis(glm::radians(time*10), glm::vec3(0.f, 1.f, 0.f));
            angle = angle * glm::angleAxis(glm::radians(time * 0.5f), glm::vec3(1, 0.f, 0.f));
            transform = transform * glm::mat4_cast(angle);
            drawCommand.transform = transform;

            skal::Engine.Renderer().Submit(drawCommand);
        }

        ImGui::ShowDemoWindow();


        skal::Engine.PresentFrame();
    }

    mesh = {};
    skal::Engine.Shutdown();
    return 0;
}
