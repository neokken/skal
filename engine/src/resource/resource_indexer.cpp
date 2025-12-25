//
// Created by okker on 22/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "skal/resource/resource_indexer.h"

#include "skal/engine.h"
#include "skal/file_io/file_io.h"

#include <filesystem>

namespace skal
{
    void ResourceIndexer::IndexDirectory(const std::string& path)
    {
        if (!std::filesystem::exists(path))
        {
            skal::Log::Error("ResourceIndexer::IndexDirectory - Directory does not exist: {}", path);
            return;
        }

        int count = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".meta")
            {
                IndexMetaFile(entry.path().string());
                count++;
            }
        }

        Log::Info("Indexed {} .meta files from {}", count, path);
    }

    void ResourceIndexer::IndexPackage(const std::string& pakPath)
    {
        Log::Error("ResourceIndexer::IndexPackage - Not implemented yet");
        // TODO (okke): Implement pak file indexing
    }

    const MetaResource* ResourceIndexer::Get(const ResourceUUID uuid) const
    {
        const auto it = m_resources.find(uuid);
        if (it == m_resources.end())
        {
            return nullptr;
        }
        return &it->second;
    }

    bool ResourceIndexer::Exists(ResourceUUID uuid) const
    {
        return m_resources.find(uuid) != m_resources.end();
    }


    std::vector<MetaResource> ResourceIndexer::ParseMetaFile(const std::string& path)
    {
        std::vector<MetaResource> results;

        if (!skal::Engine.FileIO().Exists(path))
        {
            skal::Log::Error("ResourceIndexer::ParseMetaFile - Could not find metaFile at: {}", path);
            return results;
        }

        std::string fileContent = Engine.FileIO().ReadTextFile(path);
        auto j = nlohmann::json::parse(fileContent);

        MetaFile meta;
        meta.uuid = ResourceUUID::from_string(j["uuid"]);
        meta.resourceType = ParseResourceType(j["type"]);
        meta.format = j["format"];

        // Parse embedded resources
        if (j.contains("embedded"))
        {
            for (auto& e : j["embedded"])
            {
                meta.embeddedResources.push_back({
                    ResourceUUID::from_string(e["uuid"]),
                    ParseResourceType(e["type"]),
                    e["format"],
                    e["sub_path"]
                });
            }
        }


        std::filesystem::path resourcePath = path;
        if (resourcePath.extension() == ".meta")
        {
            resourcePath.replace_extension("");
        }


        results.push_back({
             meta.uuid,
             meta.resourceType,
             meta.format,
             resourcePath.string(),
             meta.embeddedResources,
        });

        // Create MetaResource for each embedded resource
        for (const auto& embedded : meta.embeddedResources)
        {
            results.push_back({
                 embedded.uuid,
                 embedded.resourceType,
                 embedded.format,
                 meta.uuid, // Parent UUID
                 {},
            });
        }

        return results;
    }


    void ResourceIndexer::IndexMetaFile(const std::string& path)
    {
        auto resources = ParseMetaFile(path);
        for (auto& resource : resources)
        {
            auto it = m_resources.find(resource.uuid);
            if (it != m_resources.end())
            {
                const auto& existing = it->second;

                if (existing.source.index() != resource.source.index())
                {
                    Log::Error("ResourceIndexer::IndexMetaFile - UUID collision {}: different source types",
                              resource.uuid.to_string());
                    continue;
                }

                if (std::holds_alternative<std::string>(existing.source))
                {
                    const auto& path1 = std::get<std::string>(existing.source);
                    const auto& path2 = std::get<std::string>(resource.source);
                    if (path1 != path2)
                    {
                        Log::Error("ResourceIndexer::IndexMetaFile - UUID collision {}: '{}.meta' and '{}.meta'",
                                  resource.uuid.to_string(), path1, path2);
                        continue;
                    }
                }
                else if (std::holds_alternative<ResourceUUID>(existing.source))
                {
                    const auto& uuid1 = std::get<ResourceUUID>(existing.source);
                    const auto& uuid2 = std::get<ResourceUUID>(resource.source);
                    if (uuid1 != uuid2)
                    {
                        Log::Error("ResourceIndexer::IndexMetaFile - UUID collision {}: embedded in {} and {}",
                                  resource.uuid.to_string(),
                                  uuid1.to_string(), uuid2.to_string());
                        continue;
                    }
                }
                else if (std::holds_alternative<PakLocation>(existing.source))
                {
                    // TODO (okke): Implement pak collision detection
                    Log::Error("ResourceIndexer::IndexMetaFile - UUID collision detection not implemented for pak sources: {}",
                              resource.uuid.to_string());

                    continue;  // Skip to be safe
                }
            }

            m_resources[resource.uuid] = std::move(resource);
        }
    }

#ifdef SKAL_EDITOR

    nlohmann::ordered_json ResourceIndexer::SerializeMetaFile(const MetaResource& meta)
    {
        nlohmann::ordered_json j;
        j["uuid"] = meta.uuid.to_string();
        j["type"] = ResourceTypeToString(meta.resourceType);
        j["format"] = meta.format;

        if (!meta.embeddedResources.empty())
        {
            j["embedded"] = nlohmann::json::array();
            for (auto& e : meta.embeddedResources)
            {
                j["embedded"].push_back({
                    {"uuid", e.uuid.to_string()},
                    {"type", ResourceTypeToString(e.resourceType)},
                    {"format", e.format},
                    {"sub_path", e.subPath}
                });
            }
        }

        return j;
    }
#endif
} // skal
