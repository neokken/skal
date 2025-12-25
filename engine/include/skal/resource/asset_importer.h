//
// Created by okker on 25/12/2025.
//


#pragma once
#include <any>

#include "resource_types.h"

namespace skal
{
    enum class AssetOwnership
    {
        Invalid,
        Engine,
        Renderer
    };

    struct ImportedAsset
    {
        ResourceType type{ResourceType::Unknown};
        std::any payload;
        AssetOwnership ownership{AssetOwnership::Invalid};
        std::string format{"unknown"};
    };


    ImportedAsset ImportAsset(const MetaResource& meta);

    ImportedAsset ImportMesh(const MetaResource& meta);

    ImportedAsset ImportScene(const MetaResource& meta);

}
