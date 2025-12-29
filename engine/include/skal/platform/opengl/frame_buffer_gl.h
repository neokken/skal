//
// Created by okker on 29/12/2025.
//

#pragma once
#include <cstdint>

namespace skal::opengl
{

    class FrameBuffer
    {
    public:
        FrameBuffer() = default;
        FrameBuffer(uint32_t width, uint32_t height);
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        FrameBuffer(FrameBuffer&& other) noexcept;
        FrameBuffer& operator=(FrameBuffer&& other) noexcept;



        virtual void Bind();
        virtual void Unbind();
        void Resize(uint32_t newWidth, uint32_t newHeight);

        uint32_t GetColorAttachment() const { return color_attachment; }
        uint32_t GetWidth() const { return width; }
        uint32_t GetHeight() const { return height; }

    private:
        void Create();
        void Destroy();

        uint32_t fbo = 0;
        uint32_t color_attachment = 0;
        uint32_t depth_attachment = 0;
        uint32_t width = 0;
        uint32_t height = 0;
    };

}
