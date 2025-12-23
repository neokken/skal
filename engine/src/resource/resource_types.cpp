//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_types.h"
#include <magic_enum/magic_enum.hpp>

#include "skal/ecs/scene.h"
#include "skal/resource/types/scene_resource.h"
#include "skal/resource/types/texture_resource.h"
#include "skal/util/log.h"


namespace skal
{
    ResourceType ParseResourceType(const std::string &str)
    {
        const auto result = magic_enum::enum_cast<ResourceType>(str);
        if (result.has_value())
        {
            return result.value();
        }

        skal::Log::Warn("Unknown resource type: {}", str);
        return ResourceType::Unknown;
    }

    std::string ResourceTypeToString(const ResourceType type)
    {
        return std::string(magic_enum::enum_name(type));
    }


    IResource* ResourceFactory::Create(ResourceType type, ResourceUUID uuid, const std::string &format,
        const std::vector<uint8_t> &data, const std::string& sourcePath )
    {
        switch(type) {
            //case ResourceType::Texture:
            //    return new TextureResource(uuid, format, data);
            //case ResourceType::Mesh:
            //    return new MeshResource(uuid, format, data);
            //
            case ResourceType::Texture:
            {
                auto* res = new Texture(uuid, format);
                res->Load(data, sourcePath);
                return res;
            }
            case ResourceType::Scene:
            {
                auto* res = new SceneResource(uuid, format);
                res->Load(data);
                return res;
            }
            case ResourceType::Unknown:
                skal::Log::Warn("Trying to create the Unknown resource type.");
                return nullptr;
            default:
                skal::Log::Warn("Trying to create the {} resource type, but not implemented in ResourceFactory::Create", magic_enum::enum_name(type));
        }


        return nullptr;
    }
}
