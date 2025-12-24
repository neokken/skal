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

    if (format == "skel-mesh-json")
    {
        m_meshHandle = Engine.Renderer().LoadMesh(format, data);
    }
    else
    {
        Log::Error("Mesh::Load unsupported format: {}", GetFormat());
        return;
    }
}
