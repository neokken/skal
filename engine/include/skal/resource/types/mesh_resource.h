//
// Created by okker on 23/12/2025.
//

#pragma once
#include "glm/vec3.hpp"
#include "nlohmann/json_fwd.hpp"
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
        std::vector<float> tex_coord_0;       // uv
        std::vector<float> tex_coord_1;       // uv
        std::vector<float> colors;           // rgba

        std::vector<uint32_t> indices;

        static nlohmann::json Serialize(const SkalMeshData& data);

        static SkalMeshData Deserialize(const nlohmann::json& j);
    };


    class Mesh : public IResource
    {
    public:
        explicit Mesh(const ResourceUUID& uuid) : IResource(uuid){}
        ~Mesh() override;

        [[nodiscard]] ResourceType GetType() const override {return ResourceType::Mesh; }
        void Load(const ImportedAsset& asset) override;


        [[nodiscard]] RenderMeshHandle GetRenderHandle() const {return m_meshHandle;}

    private:
        friend class ResourceFactory;

        RenderMeshHandle m_meshHandle{};
    };
}
