//
// Created by okker on 30/12/2025.
//

#pragma once
#include "editor_panel_interface.h"
#include "imgui.h"
#include "skal/renderering/renderer.h"
#include "skal/util/log.h"


namespace editor
{

    class Viewport final : public IEditorPanel
    {
    public:
        Viewport();
        ~Viewport() override;

        void Render() override;

        [[nodiscard]] const char* GetName() const override {return "Viewport";}
        [[nodiscard]] bool IsOpen() const override {return true;} // can't be closed

        void SetOpen(bool) override { skal::Log::Error("Viewport::SetOpen - cant open or close this window, its always open "); }


        skal::FrameData* GetViewportFrameData();

        void SetAutoResize(bool enabled);
        void SetRenderResolution(uint32_t width, uint32_t height);
    private:
        void UpdateFramebuffer();
        void CalculateDisplayRect();

    private:
        skal::FrameData m_frameData;
        skal::RenderFrameBufferHandle m_framebuffer;

        uint32_t m_renderWidth{1280};
        uint32_t m_renderHeight{720};
        float m_renderAspect{0};

        uint32_t m_viewportWidth{0};
        uint32_t m_viewportHeight{0};
        float m_viewportAspect{0};


        bool m_autoResize{true};

        ImVec2 m_displaySize;
        ImVec2 m_displayOffset;
    };


}
