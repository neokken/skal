//
// Created by okker on 23/12/2025.
//

#pragma once
#include <cstdint>

#include "skal/renderering/renderer.h"

namespace skal
{
    class GL_Image
    {
    public:
        GL_Image() = default;
        ~GL_Image();

        GL_Image(const GL_Image&) = delete;
        GL_Image& operator=(const GL_Image&) = delete;

        GL_Image(GL_Image&&) noexcept;
        GL_Image& operator=(GL_Image&&) noexcept;

        void CreateGLTextureWithData(unsigned char* data, int width, int height, int channels,
                                      const TextureDescriptor& desc);

        uint32_t GetTextureId() const { return m_texture; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        uint32_t m_texture{0};
        int m_width{-1};
        int m_height{-1};
        int m_channels{-1};
    };
}
