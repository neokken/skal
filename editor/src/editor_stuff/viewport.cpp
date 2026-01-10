//
// Created by okker on 30/12/2025.
//

#include "viewport.h"

#include "skal/engine.h"

editor::Viewport::Viewport()
{
    m_framebuffer = skal::Engine.Renderer().CreateFrameBuffer(10,10);
}

editor::Viewport::~Viewport()
{
    skal::Engine.Renderer().DestroyFrameBuffer(m_framebuffer);
}


skal::FrameData* editor::Viewport::GetViewportFrameData()
{
    if (m_viewportWidth <= 0 || m_viewportHeight <= 0 || m_renderWidth<=0 || m_renderHeight <= 0 || !m_framebuffer.valid() )
    {
        return nullptr;
    }

    m_frameData.frame_buffer = m_framebuffer;
    m_frameData.width = static_cast<uint16_t>(m_renderWidth);
    m_frameData.height = static_cast<uint16_t>(m_renderHeight);
    m_frameData.clear_color = glm::vec4(skal::colors::lime, 1.f);
    return &m_frameData;
}

void editor::Viewport::SetAutoResize(bool enabled)
{
    m_autoResize = true;
}

void editor::Viewport::SetRenderResolution(uint32_t width, uint32_t height)
{
    m_renderWidth = width;
    m_renderHeight = height;
    m_renderAspect = static_cast<float>(width)/static_cast<float>(height);
    m_autoResize = false;
}

void editor::Viewport::UpdateFramebuffer()
{
}

void editor::Viewport::CalculateDisplayRect()
{

}

void editor::Viewport::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    ImVec2 panelSize = ImGui::GetContentRegionAvail();
    panelSize.x = panelSize.x < 0 ? 0 : panelSize.x;
    panelSize.y = panelSize.y < 0 ? 0 : panelSize.y;

    m_viewportWidth = static_cast<int>(panelSize.x );
    m_viewportHeight = static_cast<int>(panelSize.y);
    if (panelSize.y == 0)
        m_viewportAspect = 1.f;
    else
        m_viewportAspect = panelSize.x / panelSize.y;

    if (m_autoResize)
    {
        m_renderWidth = m_viewportWidth;
        m_renderHeight = m_viewportHeight;
        m_renderAspect = m_viewportAspect;
    }


    ImVec2 imageSize;
    ImVec2 offset = ImVec2(0, 0);


    // Fit while maintaining aspect ratio (with black bars)
    if (m_viewportAspect > m_renderAspect)
    {
        // Panel is too wide
        imageSize.y = panelSize.y;
        imageSize.x = panelSize.y * m_renderAspect;
        offset.x = (panelSize.x - imageSize.x) * 0.5f;
    }
    else
    {
        // Panel is too tall
        imageSize.x = panelSize.x;
        imageSize.y = panelSize.x / m_renderAspect;
        offset.y = (panelSize.y - imageSize.y) * 0.5f;
    }

    // Center the image with black bars
    ImGui::SetCursorPos(offset);


    const uint32_t texture_id = skal::Engine.Renderer().GetTextureId(m_framebuffer);
    ImGui::Image(
        reinterpret_cast<void *>(static_cast<uintptr_t>(texture_id)),
        imageSize,
        ImVec2(0, 1), // UV top-left (flipped for OpenGL)
        ImVec2(1, 0) // UV bottom-right
    );


    ImGui::End();
    ImGui::PopStyleVar();
}
