//
// Created by okker on 20/12/2025.
//

#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "skal/math/math_types.h"


namespace skal
{
    enum class RenderMeshHandle : uint32_t
    {
        null = 0
    };
    enum class RenderTextureHandle : uint32_t
    {
        null = 0
    };
    enum class RenderShaderHandle : uint32_t
    {
        null = 0
    };


    struct TextureDescriptor
    {
        enum class ColorSpace { Unorm, Srgb } color_space;
        bool generate_mipmaps;
    };

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        RenderMeshHandle LoadMesh(const std::string& format, const std::vector<uint8_t>& data);
        void UnloadMesh(RenderMeshHandle handle);

        AABB GetMeshBounds(RenderMeshHandle);

        RenderTextureHandle LoadTexture(const std::string& format, const std::vector<uint8_t>& data, const TextureDescriptor& desc);
        void UnloadTexture(RenderTextureHandle handle);

        RenderShaderHandle LoadShader(const std::string& format, const std::vector<uint8_t>& data);
        void UnloadShader(RenderShaderHandle handle);


        void Render();


    private:
        class RendererImpl;
        std::unique_ptr<RendererImpl> m_impl;
    };
}

