//
// Created by okker on 20/12/2025.
//

#include <skal/engine.h>

#include "skal/file_io.h"
#include "skal/util/log.h"


int main(const int argc, char **argv)
{
    skal::Engine.Initialize();

    std::string path{};

    // CURENTLY DOING THE SAME AS THE EDITOR
    // BUT PLAYER SHOULD PREFER RUNNING .PAK

    if (argc > 1)
    {
        // Command line: editor.exe "C:/Projects/MyGame"
        path = argv[1];
    }

    const std::string projectPath = path + "/project.skal";
    if (!skal::Engine.FileIO().Exists(projectPath))
    {
        skal::Log::Error("No project.skal found at {}", projectPath);
        skal::Engine.Shutdown();
        return 1;
    }


    // Index assets


    // Load project file
    const auto json = nlohmann::json::parse(skal::Engine.FileIO().ReadTextFile(projectPath));
    const skal::Project project = skal::DeserializeProject(json);


    skal::Engine.LoadProject(project);


    while (skal::Engine.ShouldRun())
    {
        skal::Engine.PreUpdate();
        skal::Engine.Update();
        skal::Engine.PostUpdate();

        skal::Engine.RenderScene();
        skal::Engine.PresentFrame();
    }

    skal::Engine.Shutdown();
    return 0;
}
