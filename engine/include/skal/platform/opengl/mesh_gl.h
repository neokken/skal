//
// Created by okker on 24/12/2025.
//


#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "skal/math/math_constants.h"
#include "skal/math/math_types.h"

namespace skal::opengl
{
    struct Vertex
    {
        glm::vec3 position{0.0f};
        glm::vec3 normal{vector_up};
        glm::vec4 tangent{vector_right, 1.0f};
        glm::vec2 tex_coord_0{0.0f, 0.0f};
        glm::vec2 tex_coord_1{0.0f, 0.0f};
        glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };


    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        void Upload(const MeshData& data);

        uint32_t GetVAO() const { return m_VAO; }
        uint32_t GetIndexCount() const { return m_IndexCount; }
        AABB GetBounds() const { return m_bounds; }

    private:
        static AABB CalculateBounds(const std::vector<Vertex>& vertices);

        uint32_t m_VAO = 0;
        uint32_t m_VBO = 0;
        uint32_t m_EBO = 0;
        uint32_t m_IndexCount = 0;

        AABB m_bounds{};
    };

}
