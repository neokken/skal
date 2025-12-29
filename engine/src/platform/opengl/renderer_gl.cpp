//
// Created by okker on 23/12/2025.
//


#include "skal/renderering/renderer.h"
#include "skal/platform/opengl/resource_pool_gl.h"

#include "skal/platform/opengl/image_gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "nlohmann/json.hpp"
#include "skal/platform/opengl/frame_buffer_gl.h"
#include "skal/platform/opengl/mesh_gl.h"
#include "skal/resource/types/mesh_resource.h"
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
            InitDefaultShader();
        }

        void BeginFrame(const FrameData &data);

        void EndFrame();


        void Submit(const DrawCommand &command);

        ResourcePool<opengl::Mesh> m_meshes;
        ResourcePool<opengl::FrameBuffer> m_framebuffers;
        //ResourcePool<opengl::Image> m_textures;

        uint32_t defaultShader{0};

    private:
        void InitDefaultShader();

        uint32_t CompileShaderProgram(const char *vertSrc, const char *fragSrc);

        //ResourcePool<GL_Shader> shaders;


        FrameData m_frameData{};
        std::vector<DrawCommand> m_drawCommands{};
    };

    void Renderer::RendererImpl::Submit(const DrawCommand &command)
    {
        m_drawCommands.push_back(command);
    }

    void Renderer::RendererImpl::BeginFrame(const FrameData &data)
    {
        m_frameData = data;
        m_drawCommands.clear();
    }

    void Renderer::RendererImpl::EndFrame()
    {
        if (m_frameData.width == 0 || m_frameData.height == 0)
        {
            skal::Log::Error("RendererImpl::EndFrame() called with invalid arguments");
            return; // Skip rendering entirely
        }

        glUseProgram(defaultShader);


        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), m_frameData.camera_position);
        viewMatrix = viewMatrix * glm::mat4_cast(m_frameData.camera_rotation);

        glm::mat4 projectionMatrix = glm::perspective(m_frameData.field_of_view_deg,
                                                      m_frameData.width / static_cast<float>(m_frameData.height),
                                                      m_frameData.near_plane, m_frameData.far_plane);

        auto *frame_buffer = m_framebuffers.Get(m_frameData.frame_buffer.id);
        if (frame_buffer)
        {
            frame_buffer->Resize(m_frameData.width, m_frameData.height);

            frame_buffer->Bind();
        }


        glViewport(0, 0, m_frameData.width, m_frameData.height);

        glClearColor(m_frameData.clear_color.x, m_frameData.clear_color.y, m_frameData.clear_color.z,
                     m_frameData.clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (const auto &cmd: m_drawCommands)
        {
            if (!cmd.mesh.valid()) continue;

            auto *mesh = m_meshes.Get(cmd.mesh.id);
            if (!mesh) continue;

            // Bind VAO
            glBindVertexArray(mesh->GetVAO());

            // Set model/view/projection
            const GLint modelLoc = glGetUniformLocation(defaultShader, "u_Model");
            const GLint viewLoc = glGetUniformLocation(defaultShader, "u_View");
            const GLint projLoc = glGetUniformLocation(defaultShader, "u_Projection");

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cmd.transform));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndexCount()), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        if (frame_buffer)
        {
            frame_buffer->Unbind();
        }
    }


    void Renderer::RendererImpl::InitDefaultShader()
    {
        const char *vertSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aUV;

        uniform mat4 u_Model;
        uniform mat4 u_View;
        uniform mat4 u_Projection;

        out vec2 vUV;

        void main()
        {
            vUV = aUV;
            gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
        }
    )";

        const char *fragSrc = R"(
        #version 330 core
        in vec2 vUV;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(vUV, 1.0, 1.0); // simple gradient for testing
        }
    )";

        defaultShader = CompileShaderProgram(vertSrc, fragSrc);
    }

    uint32_t Renderer::RendererImpl::CompileShaderProgram(const char *vertSrc, const char *fragSrc)
    {
        auto compile = [](GLenum type, const char *src) -> uint32_t
        {
            uint32_t shader = glCreateShader(type);
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                skal::Log::Error("Shader compilation failed: {}", infoLog);
                glDeleteShader(shader);
                return 0;
            }

            return shader;
        };

        uint32_t vertShader = compile(GL_VERTEX_SHADER, vertSrc);
        uint32_t fragShader = compile(GL_FRAGMENT_SHADER, fragSrc);

        uint32_t program = glCreateProgram();
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            skal::Log::Error("Shader program linking failed: {}", infoLog);
            glDeleteProgram(program);
            program = 0;
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return program;
    }

    // === //


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

    RenderMeshHandle Renderer::LoadMesh(const SkalMeshData &data)
    {
        auto skalMeshData = data;
        size_t vertex_count = skalMeshData.positions.size() / 3;

        if (vertex_count == 0)
        {
            skal::Log::Error("Renderer::LoadMesh - No vertex data in mesh");
            return {};
        }

        // Validate attribute counts match position count
        if (!skalMeshData.normals.empty() && skalMeshData.normals.size() != skalMeshData.positions.size())
        {
            skal::Log::Warn("Renderer::LoadMesh - Normal count mismatch, skipping normals");
            skalMeshData.normals.clear();
        }
        if (!skalMeshData.tangents.empty() && skalMeshData.tangents.size() != vertex_count * 4)
        {
            skal::Log::Warn("Renderer::LoadMesh - Tangent count mismatch, skipping tangents");
            skalMeshData.tangents.clear();
        }
        if (!skalMeshData.tex_coord_0.empty() && skalMeshData.tex_coord_0.size() != vertex_count * 2)
        {
            skal::Log::Warn("Renderer::LoadMesh - TexCoord0 count mismatch, skipping");
            skalMeshData.tex_coord_0.clear();
        }
        if (!skalMeshData.tex_coord_1.empty() && skalMeshData.tex_coord_1.size() != vertex_count * 2)
        {
            skal::Log::Warn("Renderer::LoadMesh - TexCoord1 count mismatch, skipping");
            skalMeshData.tex_coord_1.clear();
        }
        if (!skalMeshData.colors.empty() && skalMeshData.colors.size() != vertex_count * 4)
        {
            skal::Log::Warn("Renderer::LoadMesh - Color count mismatch, skipping colors");
            skalMeshData.colors.clear();
        }


        opengl::MeshData glMeshData;
        glMeshData.indices = skalMeshData.indices;
        glMeshData.vertices.reserve(vertex_count);


        for (size_t i = 0; i < vertex_count; i++)
        {
            opengl::Vertex v{};

            v.position = glm::vec3{
                skalMeshData.positions[i * 3],
                skalMeshData.positions[i * 3 + 1],
                skalMeshData.positions[i * 3 + 2]
            };

            if (!skalMeshData.normals.empty())
            {
                v.normal = glm::vec3{
                    skalMeshData.normals[i * 3],
                    skalMeshData.normals[i * 3 + 1],
                    skalMeshData.normals[i * 3 + 2]
                };
            }

            if (!skalMeshData.tangents.empty())
            {
                v.tangent = glm::vec4{
                    skalMeshData.tangents[i * 4],
                    skalMeshData.tangents[i * 4 + 1],
                    skalMeshData.tangents[i * 4 + 2],
                    skalMeshData.tangents[i * 4 + 3]
                };
            }

            if (!skalMeshData.colors.empty())
            {
                v.color = glm::vec4{
                    skalMeshData.colors[i * 4],
                    skalMeshData.colors[i * 4 + 1],
                    skalMeshData.colors[i * 4 + 2],
                    skalMeshData.colors[i * 4 + 3]
                };
            }

            if (!skalMeshData.tex_coord_0.empty())
            {
                v.tex_coord_0 = glm::vec2{
                    skalMeshData.tex_coord_0[i * 2],
                    skalMeshData.tex_coord_0[i * 2 + 1]
                };
            }

            if (!skalMeshData.tex_coord_1.empty())
            {
                v.tex_coord_1 = glm::vec2{
                    skalMeshData.tex_coord_1[i * 2],
                    skalMeshData.tex_coord_1[i * 2 + 1]
                };
            }

            glMeshData.vertices.push_back(v);
        }

        opengl::Mesh mesh;
        mesh.Upload(glMeshData);

        uint32_t id = m_impl->m_meshes.Allocate(std::move(mesh));
        return RenderMeshHandle{id};
    }

    void Renderer::UnloadMesh(const RenderMeshHandle handle)
    {
        if (!handle.valid()) return;

        m_impl->m_meshes.Free(handle.id);
    }

    RenderFrameBufferHandle Renderer::CreateFrameBuffer(uint32_t width, uint32_t height)
    {
        opengl::FrameBuffer fb(width, height);

        const uint32_t id = m_impl->m_framebuffers.Allocate(std::move(fb));
        skal::Log::Info("Renderer::CreateFrameBuffer - created frame buffer id : {}", id);

        return RenderFrameBufferHandle{id};
    }

    uint32_t Renderer::GetTextureId(RenderFrameBufferHandle handle)
    {
        const auto *fb = m_impl->m_framebuffers.Get(handle.id);
        if (!fb)
        {
            skal::Log::Error("Renderer::GetTextureId - framebuffer not found: {}", handle.id);
            return 0;
        }

        return fb->GetColorAttachment();
    }


    void Renderer::DestroyFrameBuffer(const RenderFrameBufferHandle handle)
    {
        if (!handle.valid()) return;

        m_impl->m_framebuffers.Free(handle.id);
    }

    void Renderer::BeginFrame(const FrameData &frameData)
    {
        m_impl->BeginFrame(frameData);
    }

    void Renderer::EndFrame()
    {
        m_impl->EndFrame();
    }

    void Renderer::Submit(const DrawCommand &command)
    {
        m_impl->Submit(command);
    }
}
