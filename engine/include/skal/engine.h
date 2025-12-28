//
// Created by okker on 20/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "skal/project.h"


namespace skal
{
    struct IDebugUI;
    class Renderer;
    class SceneManager;
    class ResourceManager;
    class ResourceIndexer;
    class FileIO;
    class Device;
    class Input;


    class EngineClass
    {
    public:
        void Initialize();
        void LoadProject(const skal::Project& project);     // Must be called before game loop.

        void Shutdown();

        void SetDebugUI(IDebugUI* ui);

        void PreUpdate();           // Device/input
        void GameUpdate();              // Scene logic
        void PostUpdate();          // Scene cleanup

        void RenderScene();
        void RenderDebugUI();
        void PresentFrame();


        void RequestClose();
        [[nodiscard]] bool ShouldRun() const;


        [[nodiscard]] Device& Device() { return *m_device; }
        [[nodiscard]] Input& Input() { return *m_input; }
        [[nodiscard]] FileIO& FileIO() { return *m_fileIO; }
        [[nodiscard]] ResourceIndexer& ResourceIndexer() { return *m_indexer; }
        [[nodiscard]] ResourceManager& ResourceManager() { return *m_resources; }
        [[nodiscard]] SceneManager& SceneManager() { return *m_sceneManager; }
        [[nodiscard]] Renderer& Renderer() { return *m_renderer; }

        [[nodiscard]] IDebugUI* GetDebugUI() { return m_debugUI; }

        [[nodiscard]] const Project& GetProject() const;
    private:
        skal::Project m_project{};
        bool m_projectLoaded{false};

        skal::Device* m_device{nullptr};
        skal::Input* m_input{nullptr};
        skal::FileIO* m_fileIO{nullptr};
        skal::ResourceIndexer* m_indexer{nullptr};
        skal::ResourceManager* m_resources{nullptr};

        skal::SceneManager* m_sceneManager{nullptr};
        skal::Renderer* m_renderer{nullptr};
        skal::IDebugUI* m_debugUI = nullptr;

    private:
        static void PrintBanner();
    };


    extern EngineClass Engine;
}
