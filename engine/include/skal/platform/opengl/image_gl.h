//
// Created by okker on 23/12/2025.
//

#pragma once
#include <cstdint>

#include "skal/renderering/renderer.h"

namespace skal::opengl
{
    class Image
    {
    public:
        Image() = default;
        ~Image();

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        Image(Image&&) noexcept;
        Image& operator=(Image&&) noexcept;

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
