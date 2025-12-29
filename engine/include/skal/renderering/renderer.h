//
// Created by okker on 20/12/2025.
//

#pragma once
#include <memory>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "skal/util/color_constants.h"
#include <glm/gtc/quaternion.hpp>

namespace skal
{
    struct SkalMeshData;

    struct RenderMeshHandle
    {
        uint32_t id{0};
        [[nodiscard]] bool valid() const { return id != 0; };
    };

    struct RenderTextureHandle
    {
        uint32_t id{0};
        [[nodiscard]] bool valid() const { return id != 0; };
    };

    struct RenderShaderHandle
    {
        uint32_t id{0};
        [[nodiscard]] bool valid() const { return id != 0; };
    };

    struct RenderFrameBufferHandle
    {
        uint32_t id{0};
        [[nodiscard]] bool valid() const { return id != 0; };
    };

    struct FrameData
    {
        uint16_t width{0};
        uint16_t height{0};

        RenderFrameBufferHandle frame_buffer{0};

        glm::vec4 clear_color{colors::dark_gray, 1.f};

        glm::vec3 camera_position{0.f, 0.f, 0.f};
        glm::quat camera_rotation{};

        float near_plane{0.f};
        float far_plane{0.f};
        float field_of_view_deg{45.f};
    };

    struct DrawCommand
    {
        RenderMeshHandle mesh{0};
        glm::mat4 transform{1.f};
    };




    class Renderer {
    public:
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Renderer&& other) noexcept  = delete;
        Renderer& operator=(Renderer&& other) noexcept  = delete;

        RenderMeshHandle LoadMesh(const SkalMeshData& data);
        void UnloadMesh(RenderMeshHandle handle);

        RenderFrameBufferHandle CreateFrameBuffer(uint32_t width, uint32_t height);
        uint32_t GetTextureId(RenderFrameBufferHandle handle);
        void DestroyFrameBuffer(RenderFrameBufferHandle handle);

       //RenderTextureHandle LoadTexture(const std::string& format, const std::vector<uint8_t>& data, const TextureDescriptor& desc);
       //void UnloadTexture(RenderTextureHandle handle);

       //RenderShaderHandle LoadShader(const std::string& format, const std::vector<uint8_t>& data);
       //void UnloadShader(RenderShaderHandle handle);


        void BeginFrame(const FrameData& frameData);
        void EndFrame();

        void Submit(const DrawCommand& command);

    private:
        Renderer();
        friend class EngineClass;

        class RendererImpl;
        std::unique_ptr<RendererImpl> m_impl;
    };
}

