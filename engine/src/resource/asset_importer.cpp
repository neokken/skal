//
// Created by okker on 25/12/2025.
//

#include "skal/resource/asset_importer.h"

#include "skal/engine.h"
#include "skal/file_io/file_io.h"
#include "skal/resource/types/mesh_resource.h"
#include "skal/util/json_helper.h"
#include "skal/util/log.h"

skal::ImportedAsset skal::ImportAsset(const MetaResource& meta)
{
    switch (meta.resourceType)
    {
    case ResourceType::Scene:
        return ImportScene(meta);
    case ResourceType::Texture:
        break;
    case ResourceType::Mesh:
        return ImportMesh(meta);
    case ResourceType::Unknown:
        break;
    default: ;
    }

    Log::Error("ImportAsset - Unknown resource type {} ", ResourceTypeToString(meta.resourceType));
    return {};
}

skal::ImportedAsset skal::ImportMesh(const MetaResource& meta)
{
    ImportedAsset asset;
    asset.type = ResourceType::Mesh;
    asset.format = meta.format;

    if (meta.format == "skal-mesh-json")
    {
        asset.ownership = skal::AssetOwnership::Engine;

        if (!std::holds_alternative<std::string>(meta.source))
        {
            skal::Log::Error("ImportMesh - Invalid source specified for skal-mesh-json, its not a path");
            return {};
        }

        const auto json_opt = skal::TryParseJson(skal::Engine.FileIO().ReadBinaryFile(std::get<std::string>(meta.source)));
        if (!json_opt)
        {
            skal::Log::Error("ImportMesh - Failed to parse JSON");
            return {};
        }

        SkalMeshData skalMeshData = SkalMeshData::Deserialize(*json_opt);

        asset.payload = skalMeshData;
        return asset;
    }
    else
    {
        Log::Error("ImportMesh - Unknown mesh format type {} ", meta.format);
    }

    return asset;
}

skal::ImportedAsset skal::ImportScene(const MetaResource& meta)
{
    ImportedAsset asset;
    asset.type = ResourceType::Scene;
    asset.format = meta.format;

    if (meta.format == "skal-scene-json")
    {
        asset.ownership = skal::AssetOwnership::Engine;

        if (std::holds_alternative<std::string>(meta.source))
        {
            auto json_opt = TryParseJson(Engine.FileIO().ReadBinaryFile(std::get<std::string>(meta.source)));
            if (!json_opt)
            {
                skal::Log::Error("ImportScene - Failed to parse JSON");
                return {};
            }

            asset.payload = *json_opt;
            return asset;

        }

        Log::Error("ImportScene - meta.source is not a path, currently unimplemented");
        return asset;
    }
    else
    {
        Log::Error("ImportScene - Unknown scene format type {} ", meta.format);
    }

    return {};
}
