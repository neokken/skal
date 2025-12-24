//
// Created by okker on 23/12/2025.
//


#include "skal/renderering/renderer.h"
#include "skal/platform/opengl/resource_pool_gl.h"

#include "skal/platform/opengl/image_gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"
#include "skal/util/log.h"


#ifdef SKAL_SDL3
#include <SDL3/SDL_video.h>
#endif

namespace skal
{
    class Renderer::RendererImpl
    {
    public:
        RendererImpl()
        {

            InitQuad();
            InitShader();
        }

        ResourcePool<GL_Image> textures;
        //ResourcePool<GL_Mesh> meshes;
        //ResourcePool<GL_Shader> shaders;


        // Internal quad rendering
        uint32_t quadVAO{0};
        uint32_t quadVBO{0};
        uint32_t quadShader{0};

    private:
        uint32_t quadEBO{0};


        void InitQuad()
        {
            float vertices[] = {
                // pos(2)      uv(2)
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 0.0f, 1.0f
            };

            uint32_t indices[] = {0, 1, 2, 2, 3, 0};

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glGenBuffers(1, &quadEBO);

            glBindVertexArray(quadVAO);

            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // Position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

            // UV
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        }

        void InitShader()
        {
            const char *vertSrc = R"(
            #version 330 core
            layout(location = 0) in vec2 aPos;
            layout(location = 1) in vec2 aUV;
            out vec2 vUV;
            void main() {
                vUV = aUV;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";

            const char *fragSrc = R"(
            #version 330 core
            in vec2 vUV;
            out vec4 FragColor;
            uniform sampler2D uTexture;
            void main() {
                FragColor = texture(uTexture, vUV);
            }
        )";

            quadShader = CompileShaderProgram(vertSrc, fragSrc);
        }

        uint32_t CompileShaderProgram(const char *vertSrc, const char *fragSrc)
        {
            // Compile vertex shader
            uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertShader, 1, &vertSrc, nullptr);
            glCompileShader(vertShader);

            int success;
            glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
                skal::Log::Error("Vertex shader compilation failed: {}", infoLog);
                return 0;
            }

            // Compile fragment shader
            uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragShader, 1, &fragSrc, nullptr);
            glCompileShader(fragShader);

            glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
                skal::Log::Error("Fragment shader compilation failed: {}", infoLog);
                glDeleteShader(vertShader);
                return 0;
            }

            // Link program
            uint32_t program = glCreateProgram();
            glAttachShader(program, vertShader);
            glAttachShader(program, fragShader);
            glLinkProgram(program);

            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetProgramInfoLog(program, 512, nullptr, infoLog);
                skal::Log::Error("Shader program linking failed: {}", infoLog);
                glDeleteShader(vertShader);
                glDeleteShader(fragShader);
                return 0;
            }

            glDeleteShader(vertShader);
            glDeleteShader(fragShader);

            return program;
        }

    };


    Renderer::Renderer()
    {
        // GLAD is GL-specific, so GL renderer handles it
#ifdef SKAL_SDL3
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        {
            skal::Log::Critical("Renderer::Renderer - Failed to initialize GLAD");
            skal::Log::Critical("Renderer::Renderer - SDL Error: {}", SDL_GetError());
            return;
        }
        skal::Log::Info("OpenGL Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
#endif

        m_impl = std::make_unique<RendererImpl>();
    }

    Renderer::~Renderer() = default;

    RenderMeshHandle Renderer::LoadMesh(const std::string &format, const std::vector<uint8_t> &data)
    {
        return {};
    }

    RenderTextureHandle Renderer::LoadTexture(const std::string &format, const std::vector<uint8_t> &data,
                                        const TextureDescriptor &desc)
    {
        if (format == "png")
        {
            int width, height, channels;
            unsigned char *pixels = stbi_load_from_memory(
                data.data(),
                static_cast<int>(data.size()),
                &width,
                &height,
                &channels,
                4               // TODO (okke) : force it to 4 channels
            );


            if (!pixels)
            {
                skal::Log::Error("Renderer::LoadTexture - Failed to load PNG texture: {}", stbi_failure_reason());
                return RenderTextureHandle::null;
            }

            if (channels != 4)
            {
                Log::Warn("Renderer::LoadTexture - Image has {} channels, converting to RGBA", channels);
            }



            GL_Image image;
            image.CreateGLTextureWithData(pixels, width, height, 4, desc);  // we force to 4 channels because of stbi_load_from_memory

            stbi_image_free(pixels); // Free stb_image allocation

            uint32_t id = m_impl->textures.Allocate(std::move(image));
            return static_cast<RenderTextureHandle>(id);
        } else
        {
            skal::Log::Error("Renderer::LoadTexture - doesnt support texture format: {}", format);
            return {};
        }
    }

    RenderShaderHandle Renderer::LoadShader(const std::string &format, const std::vector<uint8_t> &data)
    {
        return {};
    }


    void Renderer::UnloadMesh(RenderMeshHandle handle)
    {
        //m_impl->meshes.Free(static_cast<uint32_t>(handle));
    }


    void Renderer::UnloadTexture(RenderTextureHandle handle)
    {
        m_impl->textures.Free(static_cast<uint32_t>(handle));
    }

    void Renderer::UnloadShader(RenderShaderHandle handle)
    {
    }

    void Renderer::Render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Get first texture for testing
        const GL_Image* image = m_impl->textures.Get(1);
        if (!image || image->GetHeight() == -1) return;

        glUseProgram(m_impl->quadShader);
        glBindTexture(GL_TEXTURE_2D, image->GetTextureId());
        glBindVertexArray(m_impl->quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
