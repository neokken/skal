//
// Created by okker on 20/12/2025.
//

#include <iostream>
#include <skal/engine.h>

#include "editor_context.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "skal/uuid_types.h"
#include "skal/input/input.h"
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

    // == game loop ==

    // TODO (okke) : temp code remove
    auto mesh = skal::Engine.ResourceManager().Load<skal::Mesh>(skal::ResourceUUID::from_string("981f34b1-cc32-4bac-b094-6a2bbfb06ed3"));
    float time{0};


    while (skal::Engine.ShouldRun())
    {
        skal::Engine.PreUpdate();

        if (editor.GetPlayState() == editor::EditorContext::PlayState::Playing)
        {
            skal::Engine.Update();
        }


        if (skal::Engine.Input().GetKeyDown(skal::input::KeyboardKey::Escape))
        {
            skal::Log::Info("Requesting Shutdown");
            skal::Engine.RequestClose();
        }



        skal::Engine.PostUpdate();

        skal::Engine.RenderScene();
        //editorCtx.RenderGizmos();
        //editorCtx.RenderUI();

        time += 1/60.f;
        if (mesh.IsValid())
        {
            skal::DrawCommand drawCommand;
            drawCommand.mesh = mesh.Get()->GetRenderHandle();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -2.f));
            glm::quat angle = glm::angleAxis(glm::radians(time), glm::vec3(0.f, 1.f, 0.f));
            angle = angle * glm::angleAxis(glm::radians(time*0.2f), glm::vec3(1,0.f,0.f));
            transform = transform * glm::mat4_cast(angle); // note: translate * rotate
            drawCommand.transform = transform;

            skal::Engine.Renderer().Submit(drawCommand);
        }


        skal::Engine.RenderDebugUI();
        skal::Engine.PresentFrame();
    }

    skal::Engine.Shutdown();
    return 0;
}
