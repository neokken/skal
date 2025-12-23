//
// Created by okker on 22/12/2025.
//

#pragma once
#include "resource_interface.h"
#include "nlohmann/json_fwd.hpp"
#include "nlohmann/detail/meta/std_fs.hpp"
#include "skal/uuid_types.h"

namespace skal
{
    class ResourceIndexer
    {
    public:
        void IndexDirectory(const std::string &path);

        void IndexPackage(const std::string &pakPath);
        void IndexMetaFile(const std::string &path);

        const MetaResource *Get(ResourceUUID uuid) const;

        bool Exists(ResourceUUID uuid) const;

#ifdef SKAL_EDITOR
        void RemoveDependent(ResourceUUID resource, ResourceUUID dependent);

        void AddDependent(ResourceUUID resource, ResourceUUID dependent);


        static nlohmann::ordered_json SerializeMetaFile(const MetaResource& meta);
#endif

    private:
        std::unordered_map<ResourceUUID, MetaResource> m_resources;

        std::vector<MetaResource> ParseMetaFile(const std::string &path);

    };
} // skal
