//
// Created by okker on 23/12/2025.
//


#include "skal/platform/opengl/image_gl.h"

#include <glad/glad.h>

#include "skal/util/log.h"

skal::GL_Image::~GL_Image()
{
    if (m_texture) glDeleteTextures(1, &m_texture);
}

skal::GL_Image::GL_Image(GL_Image&& other) noexcept
    : m_texture(other.m_texture)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_channels(other.m_channels)
    {
        other.m_texture = 0;
        other.m_width = -1;
        other.m_height = -1;
        other.m_channels = -1;
    }

skal::GL_Image& skal::GL_Image::operator=(GL_Image&& other) noexcept
{
    if (this != &other)
    {
        if (m_texture) glDeleteTextures(1, &m_texture);

        m_texture = other.m_texture;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;

        other.m_texture = 0;
        other.m_width = -1;
        other.m_height = -1;
        other.m_channels = -1;
    }
    return *this;
}

void skal::GL_Image::CreateGLTextureWithData(unsigned char *data, int width, int height, int channels,
                                          const TextureDescriptor &desc)
{
    if (m_texture)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }

    m_width = width;
    m_height = height;
    m_channels = channels;

    GLint internalFormat = GL_INVALID_VALUE;
    GLint usage = GL_INVALID_VALUE;

    switch (channels)
    {
        case 1:
            internalFormat = GL_R8;
            usage = GL_RED;
            break;
        case 4:
            internalFormat = (desc.color_space == TextureDescriptor::ColorSpace::Srgb)
                ? GL_SRGB8_ALPHA8
                : GL_RGBA8;
            usage = GL_RGBA;
            break;
        default:
            Log::Error("Image::CreateGLTextureWithData - Unsupported channel count: {}", channels);
            return;
    }


    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    skal::Log::Info("Creating texture: {}x{}, {} channels, format={}, usage={}",
                    width, height, channels, internalFormat, usage);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0,
                 usage, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        Log::Error("Image::CreateGLTextureWithData - glTexImage2D failed: {}", error);
        m_texture = 0;
        return;
    }

    if (desc.generate_mipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}
