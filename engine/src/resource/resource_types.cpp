//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_types.h"
#include <magic_enum/magic_enum.hpp>

#include "skal/ecs/scene.h"
#include "skal/resource/asset_importer.h"
#include "skal/resource/types/mesh_resource.h"
#include "skal/resource/types/scene_resource.h"
#include "skal/resource/types/texture_resource.h"
#include "skal/util/log.h"


namespace skal
{
    ResourceType ParseResourceType(const std::string& str)
    {
        const auto result = magic_enum::enum_cast<ResourceType>(str);
        if (result.has_value())
        {
            return result.value();
        }

        skal::Log::Error("ParseResourceType - Cannot parse ResourceType: '{}'", str);
        return ResourceType::Unknown;
    }

    std::string ResourceTypeToString(const ResourceType type)
    {
        return std::string(magic_enum::enum_name(type));
    }

    IResource* ResourceFactory::Create(const ResourceUUID& uuid, const ImportedAsset& asset)
    {
        switch (asset.type)
        {
        case ResourceType::Scene:
            {
                auto* res = new SceneResource(uuid);
                res->Load(asset);
                return res;
            }
        case ResourceType::Mesh:
            {
                auto* res = new Mesh(uuid);
                res->Load(asset);
                return res;
            }
        case ResourceType::Unknown:
            skal::Log::Error("ResourceFactory::Create - Trying to create the Unknown resource type.");
            break;

        case ResourceType::Texture:
        default: ;
            skal::Log::Error("ResourceFactory::Create - Trying to create the {} resource type, but not implemented", ResourceTypeToString(asset.type));
        }

        return nullptr;
    }

}