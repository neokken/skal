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


    while (skal::Engine.ShouldRun())
    {
        skal::Engine.PreUpdate();

        if (editor.GetPlayState() == editor::EditorContext::PlayState::Playing)
        {
            skal::Engine.GameUpdate();
        }

        skal::Engine.PostUpdate();

        skal::Engine.RenderScene();

        skal::Engine.RenderDebugUI();
        skal::Engine.PresentFrame();
    }

    skal::Engine.Shutdown();
    return 0;
}
