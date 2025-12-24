//
// Created by okker on 23/12/2025.
//

#pragma once
#include "glm/vec3.hpp"
#include "skal/math/math_types.h"
#include "skal/renderering/renderer.h"
#include "skal/resource/resource_interface.h"

namespace skal
{

    struct SkalMeshData
    {
        std::vector<float> positions;        // xyz
        std::vector<float> normals;          // xyz
        std::vector<float> tangents;         // xyzw
        std::vector<float> uvs;              // uv
        std::vector<float> colors;           // rgba

        std::vector<uint32_t> indices;
    };



    class Mesh : public IResource
    {
    public:
        Mesh(const ResourceUUID& uuid, const std::string& format) : IResource(uuid, format){}
        ~Mesh() override;

        [[nodiscard]] ResourceType GetType() const override {return ResourceType::Mesh; }


    private:
        friend class ResourceFactory;
        void Load(const std::vector<uint8_t>& data);

        RenderMeshHandle m_meshHandle{};

        AABB m_bounds{};
    };
}
