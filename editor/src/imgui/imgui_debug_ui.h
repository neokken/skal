//
// Created by okker on 28/12/2025.
//

#pragma once
#include "skal/util/debug_ui.h"


struct NativeWindowHandle;

namespace editor
{
    class ImGuiDebugUI final : public skal::IDebugUI
    {
    public:
        ImGuiDebugUI() = delete;
        explicit ImGuiDebugUI(const NativeWindowHandle& handle);

        void Initialize() override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;

        void ProcessSDLEvent(const SDL_Event& e) override;

    private:
        SDL_Window* m_window = nullptr;
        void* m_glContext = nullptr;
    };
}
