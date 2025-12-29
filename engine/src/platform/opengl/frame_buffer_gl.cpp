//
// Created by okker on 29/12/2025.
//

#include "skal/platform/opengl/frame_buffer_gl.h"

#include "glad/glad.h"

skal::opengl::FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
    : width(width), height(height)
{
    Create();
}

skal::opengl::FrameBuffer::~FrameBuffer()
{
    Destroy();
}

skal::opengl::FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
    : fbo(other.fbo)
      , color_attachment(other.color_attachment)
      , depth_attachment(other.depth_attachment)
      , width(other.width)
      , height(other.height)
{
    other.fbo = 0;
    other.color_attachment = 0;
    other.depth_attachment = 0;
}

skal::opengl::FrameBuffer & skal::opengl::FrameBuffer::operator=(FrameBuffer &&other) noexcept
{
    if (this != &other) {
        Destroy();

        fbo = other.fbo;
        color_attachment = other.color_attachment;
        depth_attachment = other.depth_attachment;
        width = other.width;
        height = other.height;

        other.fbo = 0;
        other.color_attachment = 0;
        other.depth_attachment = 0;
    }
    return *this;
}

void skal::opengl::FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void skal::opengl::FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void skal::opengl::FrameBuffer::Resize(const uint32_t newWidth, const uint32_t newHeight)
{
    if (width == newWidth && height == newHeight) return;

    width = newWidth;
    height = newHeight;
    Destroy();
    Create();
}

void skal::opengl::FrameBuffer::Create()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &color_attachment);
    glBindTexture(GL_TEXTURE_2D, color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);

    glGenRenderbuffers(1, &depth_attachment);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_attachment);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void skal::opengl::FrameBuffer::Destroy()
{
    if (fbo)
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &color_attachment);
        glDeleteRenderbuffers(1, &depth_attachment);
        fbo = color_attachment = depth_attachment = 0;
    }
}
