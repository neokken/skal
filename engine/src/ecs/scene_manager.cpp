//
// Created by okker on 22/12/2025.
//

#include "skal/ecs/scene_manager.h"

#include "skal/engine.h"
#include "skal/ecs/scene.h"
#include "skal/resource/resource_manager.h"
#include "skal/resource/types/scene_resource.h"
#include "skal/util/log.h"

void skal::SceneManager::LoadScene(const ResourceUUID sceneUUID)
{
    const auto scene = Engine.ResourceManager().Load<SceneResource>(sceneUUID);
    if (!scene.IsValid()) {
        Log::Error("Could not load scene resource");
        return;
    }


    m_activeScene = std::make_unique<Scene>();
    m_sceneResource = scene;

    m_activeScene->Deserialize(m_sceneResource.Get()->GetData());
}
