//
// Created by okker on 22/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeConst
#pragma once
#include <memory>

#include "skal/uuid_types.h"
#include "skal/resource/resource_handle.h"


namespace skal
{
    class SceneResource;
    class Scene;


    class SceneManager
    {
    public:
        void LoadScene(ResourceUUID sceneUUID);

        Scene* GetActiveScene() { return m_activeScene.get(); }

    private:
        std::unique_ptr<Scene> m_activeScene;
        Handle<SceneResource> m_sceneResource;  // keeps resource loaded
    };
}
