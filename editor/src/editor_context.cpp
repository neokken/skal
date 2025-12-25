//
// Created by okker on 21/12/2025.
//

#include "editor_context.h"
#include <filesystem>


#include "skal/engine.h"
#include "skal/file_io/file_io.h"
#include "skal/project.h"
#include "skal/uuid_types.h"
#include "skal/ecs/scene.h"
#include "skal/resource/resource_indexer.h"
#include "skal/resource/resource_types.h"
#include "skal/util/log.h"


namespace editor
{
    bool EditorContext::CreateProject(const std::string &basePath, const std::string &name)
    {
        // creating base structure
        std::string projectPath = basePath + "/" + name;

        std::filesystem::create_directories(projectPath);
        std::filesystem::create_directories(projectPath + "/data");
        std::filesystem::create_directories(projectPath + "/assets");
        std::filesystem::create_directories(projectPath + "/assets/scenes");
        std::filesystem::create_directories(projectPath + "/assets/models");
        std::filesystem::create_directories(projectPath + "/assets/textures");
        std::filesystem::create_directories(projectPath + "/.editor");

        // copy engine assets  TODO (okke): but copy instead to here
        std::filesystem::create_directories(projectPath + "/assets/engine");



        const skal::ResourceUUID defaultSceneUUID = skal::UUIDGenerator::generate<skal::ResourceUUID>();

        skal::Scene newScene{};

        skal::MetaResource meta;
        meta.uuid = defaultSceneUUID;
        meta.resourceType = skal::ResourceType::Scene;
        meta.format = "base";


        const nlohmann::ordered_json metaJson = skal::ResourceIndexer::SerializeMetaFile(meta);

        skal::Engine.FileIO().WriteTextFile(projectPath + "/assets/scenes/default.scene", newScene.Serialize().dump(4));
        skal::Engine.FileIO().WriteTextFile(projectPath + "/assets/scenes/default.scene.meta", metaJson.dump(4));


        skal::Project project{
            name,
            SKAL_ENGINE_VERSION,
             defaultSceneUUID
        };

        const auto json = skal::SerializeProject(project);

        skal::Engine.FileIO().WriteTextFile(projectPath + "/project.skal", json.dump(4));

        skal::Log::Info("Created project: {} at {}", name, basePath);
        return true;
    }

    bool EditorContext::OpenProject(const std::string &path)
    {
        m_projectPath = path + "/project.skal";
        if (!skal::Engine.FileIO().Exists(m_projectPath))
        {
            skal::Log::Error("EditorContext::OpenProject No project.skal found at {}", path);
            return false;
        }


        // Index assets

        skal::Engine.ResourceIndexer().IndexDirectory(path + "/assets");


        // Load project file
        const auto json = nlohmann::json::parse(skal::Engine.FileIO().ReadTextFile(m_projectPath));
        skal::Project project = skal::DeserializeProject(json);


        // Load editor settings for this project
        //LoadEditorSettings(path + "/.editor");

        skal::Engine.LoadProject(project);


        skal::Log::Info("Opened project: {}", project.name);
        m_projectOpen = true;
        return true;
    }

    void EditorContext::SetPlayState(const PlayState state)
    {
        m_playState = state;
    }
} // editor
