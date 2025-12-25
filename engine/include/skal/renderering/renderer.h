//
// Created by okker on 20/12/2025.
//

#pragma once
#include <memory>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "skal/util/color_constants.h"


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

    struct TextureDescriptor
    {
        enum class ColorSpace { Unorm, Srgb } color_space;
        bool generate_mipmaps;
    };


    struct FrameData
    {
        uint16_t width{0};
        uint16_t height{0};

        glm::vec4 clear_color{colors::dark_gray, 1.f};

        glm::mat4 view{1.f};
        glm::mat4 projection{1.f};
    };

    struct DrawCommand
    {
        RenderMeshHandle mesh{0};
        glm::mat4 transform{1.f};
    };



    class Renderer {
    public:
        Renderer();
        ~Renderer();

        RenderMeshHandle LoadMesh(const SkalMeshData& data);
        void UnloadMesh(RenderMeshHandle handle);

       //RenderTextureHandle LoadTexture(const std::string& format, const std::vector<uint8_t>& data, const TextureDescriptor& desc);
       //void UnloadTexture(RenderTextureHandle handle);

       //RenderShaderHandle LoadShader(const std::string& format, const std::vector<uint8_t>& data);
       //void UnloadShader(RenderShaderHandle handle);


        void BeginFrame(const FrameData& frameData);
        void EndFrame();

        void Submit(const DrawCommand& command);

    private:

        class RendererImpl;
        std::unique_ptr<RendererImpl> m_impl;
    };
}

