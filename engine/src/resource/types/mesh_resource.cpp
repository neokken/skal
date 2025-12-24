//
// Created by okker on 23/12/2025.
//

#include "skal/resource/types/mesh_resource.h"

#include "skal/engine.h"


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
