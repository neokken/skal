//
// Created by okker on 20/12/2025.
//

#pragma once
#include <cstdint>
#include <memory>
#include <vector>


namespace skal
{
    enum class MeshHandle : uint32_t
    {
        null = 0
    };
    enum class TextureHandle : uint32_t
    {
        null = 0
    };
    enum class ShaderHandle : uint32_t
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

        MeshHandle LoadMesh(const std::string& format, const std::vector<uint8_t>& data);
        void UnloadMesh(MeshHandle handle);

        TextureHandle LoadTexture(const std::string& format, const std::vector<uint8_t>& data, const TextureDescriptor& desc);
        void UnloadTexture(TextureHandle handle);

        ShaderHandle LoadShader(const std::string& format, const std::vector<uint8_t>& data);
        void UnloadShader(ShaderHandle handle);


        void Render();


    private:
        class RendererImpl;
        std::unique_ptr<RendererImpl> m_impl;
    };
}

