//
// Created by okker on 24/12/2025.
//

#include "skal/platform/opengl/mesh_gl.h"

#include "glad/glad.h"
#include "skal/util/log.h"

skal::opengl::Mesh::~Mesh()
{
    if (m_VAO != 0)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0)
        glDeleteBuffers(1, &m_VBO);
    if (m_EBO != 0)
        glDeleteBuffers(1, &m_EBO);
}

skal::opengl::Mesh::Mesh(Mesh&& other) noexcept
    : m_VAO(other.m_VAO)
    , m_VBO(other.m_VBO)
    , m_EBO(other.m_EBO)
    , m_IndexCount(other.m_IndexCount)
    , m_bounds(other.m_bounds)
{
    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_EBO = 0;
    other.m_IndexCount = 0;
}

skal::opengl::Mesh& skal::opengl::Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
        if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);

        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_EBO = other.m_EBO;
        m_IndexCount = other.m_IndexCount;
        m_bounds = other.m_bounds;

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_EBO = 0;
        other.m_IndexCount = 0;
    }
    return *this;
}


void skal::opengl::Mesh::Upload(const opengl::MeshData& data)
{
    if (data.vertices.empty())
    {
        skal::Log::Error("Mesh::Upload - Cannot upload empty mesh data");
        return;
    }

    if (m_VAO != 0)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0)
        glDeleteBuffers(1, &m_VBO);
    if (m_EBO != 0)
        glDeleteBuffers(1, &m_EBO);

    m_IndexCount = static_cast<uint32_t>(data.indices.size());

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Upload vertex buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 data.vertices.size() * sizeof(Vertex),
                 data.vertices.data(),
                 GL_STATIC_DRAW);

    // Vertex attribute layout
    // Position (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));

    // Normal (location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));

    // Tangent (location 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, tangent));

    // UV0 (location 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, tex_coord_0));

    // UV1 (location 4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, tex_coord_1));

    // Color (location 5)
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, color));

    // Upload index buffer if present
    if (!data.indices.empty())
    {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     data.indices.size() * sizeof(uint32_t),
                     data.indices.data(),
                     GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    m_bounds = CalculateBounds(data.vertices);
}

skal::AABB skal::opengl::Mesh::CalculateBounds(const std::vector<Vertex>& vertices)
{
    AABB result;

    for (const auto& vert : vertices)
    {
        if (vert.position.x < result.min.x) result.min.x = vert.position.x;
        if (vert.position.y < result.min.y) result.min.y = vert.position.y;
        if (vert.position.z < result.min.z) result.min.z = vert.position.z;

        if (vert.position.x > result.max.x) result.max.x = vert.position.x;
        if (vert.position.y > result.max.y) result.max.y = vert.position.y;
        if (vert.position.z > result.max.z) result.max.z = vert.position.z;
    }
    return result;
}
