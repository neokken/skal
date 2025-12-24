//
// Created by okker on 23/12/2025.
//

#include "skal/resource/types/mesh_resource.h"

#include "skal/engine.h"


nlohmann::json skal::SkalMeshData::Serialize(const SkalMeshData& data)
{
    nlohmann::json j;

    if (!data.positions.empty()) j["positions"] = data.positions;
    if (!data.normals.empty()) j["normals"] = data.normals;
    if (!data.tangents.empty()) j["tangents"] = data.tangents;
    if (!data.tex_coord_0.empty()) j["tex_coord_0"] = data.tex_coord_0;
    if (!data.tex_coord_1.empty()) j["tex_coord_1"] = data.tex_coord_1;
    if (!data.colors.empty()) j["colors"] = data.colors;
    if (!data.indices.empty()) j["indices"] = data.indices;

    return j;
}

skal::SkalMeshData skal::SkalMeshData::Deserialize(const nlohmann::json& j)
{
    SkalMeshData data;

    if (j.contains("positions"))
    {
        data.positions = j["positions"].get<std::vector<float>>();
    }

    if (j.contains("normals"))
    {
        data.normals = j["normals"].get<std::vector<float>>();
    }

    if (j.contains("tangents"))
    {
        data.tangents = j["tangents"].get<std::vector<float>>();
    }

    if (j.contains("tex_coord_0"))
    {
        data.tex_coord_0 = j["tex_coord_0"].get<std::vector<float>>();
    }

    if (j.contains("tex_coord_1"))
    {
        data.tex_coord_1 = j["tex_coord_1"].get<std::vector<float>>();
    }

    if (j.contains("colors"))
    {
        data.colors = j["colors"].get<std::vector<float>>();
    }

    if (j.contains("indices"))
    {
        data.indices = j["indices"].get<std::vector<uint32_t>>();
    }

    return data;
}

skal::Mesh::~Mesh()
{
    skal::Engine.Renderer().UnloadMesh(m_meshHandle);
}

void skal::Mesh::Load(const std::vector<uint8_t>& data)
{
    const auto& format = GetFormat();

    if (format == "skal-mesh-json")
    {
        m_meshHandle = Engine.Renderer().LoadMesh(GetFormat(), data);

        if (m_meshHandle == RenderMeshHandle::null)
        {
            Log::Error("Mesh::Load - Failed to load mesh");
            return;
        }

        m_bounds = Engine.Renderer().GetMeshBounds(m_meshHandle);
    }
    else
    {
        Log::Error("Mesh::Load - Unsupported format: {}", GetFormat());
        return;
    }
}
