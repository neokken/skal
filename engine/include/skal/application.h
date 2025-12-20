//
// Created by okker on 20/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeConst
#pragma once



namespace skal
{
    class Device;
    class Input;

    class Application
    {
    public:
        void Initialize(const char *projectPath);
        void Shutdown();

        void UpdateSystems();
        void RenderScene();    // Scene rendering
        void RenderDebugUI();  // Engine debug overlays
        void PresentFrame();   // Swap buffers

        void RequestClose();
        [[nodiscard]] bool ShouldRun() const;


        [[nodiscard]] Device& Device() { return *m_device; }
        [[nodiscard]] Input& Input() { return *m_input; }
    private:
        skal::Device* m_device{nullptr};
        skal::Input* m_input{nullptr};
        // Renderer* m_renderer = nullptr;
        // FileIO* m_fileIO = nullptr


    private:
        static void PrintBanner();
    };


    extern Application Engine;
}
