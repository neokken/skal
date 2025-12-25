//
// Created by okker on 22/12/2025.
//

#pragma once
#include <variant>
#include "skal/uuid_types.h"


namespace skal
{
    struct ImportedAsset;
    class IResource;

    enum class ResourceType
    {
        Scene,
        Texture,
        Mesh,
        Unknown,
        // Add new types here
    };

    // Conversion utilities
    ResourceType ParseResourceType(const std::string& str);
    std::string ResourceTypeToString(ResourceType type);


    struct EmbeddedMetaResource
    {
        ResourceUUID uuid;
        ResourceType resourceType;
        std::string format;
        std::string subPath;
    };


    struct MetaFile
    {
        ResourceUUID uuid;
        ResourceType resourceType;
        std::string format;

        std::vector<EmbeddedMetaResource> embeddedResources;
    };

    struct PakLocation
    {
        std::string pakPath;
        uint64_t offset;
        uint64_t size;
    };

    struct MetaResource
    {
        ResourceUUID uuid;
        ResourceType resourceType;
        std::string format;

        // Where to load from
        std::variant<
            std::string, // File path (editor)
            ResourceUUID, // Parent UUID (embedded)
            PakLocation // Pak file (player)
        > source;

        std::vector<EmbeddedMetaResource> embeddedResources;
    };


    class ResourceFactory
    {
    public:
        static IResource* Create(
            const ResourceUUID& uuid,
            const ImportedAsset& asset
        );
    };
}
