//
// Created by okker on 22/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "skal/resource/resource_indexer.h"

#include "skal/engine.h"
#include "skal/file_io.h"

#include <filesystem>

namespace skal
{
    void ResourceIndexer::IndexDirectory(const std::string &path)
    {
        if (!std::filesystem::exists(path)) {
            skal::Log::Error("ResourceIndexer::IndexDirectory, Directory does not exist: {}", path);
            return;
        }

        int count = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".meta") {
                IndexMetaFile(entry.path().string());
                count++;
            }
        }

        Log::Info("Indexed {} .meta files from {}", count, path);
    }

    void ResourceIndexer::IndexPackage(const std::string &pakPath)
    {
        Log::Critical("Packaging not implemented yet"); //TODO (okke): packaging and indexing them
    }

    const MetaResource* ResourceIndexer::Get(const ResourceUUID uuid) const
    {
        const auto it = m_resources.find(uuid);
        if (it == m_resources.end()) {
            return nullptr;
        }
        return &it->second;
    }

    bool ResourceIndexer::Exists(ResourceUUID uuid) const
    {
        return m_resources.find(uuid) != m_resources.end();
    }


    std::vector<MetaResource> ResourceIndexer::ParseMetaFile(const std::string &path)
    {
        std::vector<MetaResource> results;

        if (!skal::Engine.FileIO().Exists(path))
        {
            skal::Log::Warn("Could not find metaFile at: {}", path);
            return results;
        }

        std::string fileContent = Engine.FileIO().ReadTextFile(path);
        auto j = nlohmann::json::parse(fileContent);

        MetaFile meta;
        meta.uuid = ResourceUUID::from_string(j["uuid"]);
        meta.resourceType = ParseResourceType(j["type"]);
        meta.format = j["format"];

        // Parse embedded resources
        if (j.contains("embedded")) {
            for (auto& e : j["embedded"]) {
                meta.embeddedResources.push_back({
                    .uuid = ResourceUUID::from_string(e["uuid"]),
                    .resourceType = ParseResourceType(e["type"]),
                    .format = e["format"],
                    .subPath = e["sub_path"]
                });
            }
        }

        // Parse dependents
        if (j.contains("dependents")) {
            for (auto& d : j["dependents"]) {
                meta.dependents.push_back(ResourceUUID::from_string(d));
            }
        }

        if (j.contains("external_dependencies_extensions")) {
            for (auto& ede : j["external_dependencies_extensions"]) {
                meta.external_dependencies_extensions.push_back(ede);
            }
        }

        std::filesystem::path resourcePath = path;
        if (resourcePath.extension() == ".meta")
        {
            resourcePath.replace_extension("");
        }


        results.push_back({
            .uuid = meta.uuid,
            .resourceType = meta.resourceType,
            .format = meta.format,
            .source = resourcePath.string(),
            .embeddedResources = meta.embeddedResources,
            .dependents = meta.dependents,
            .external_dependencies_extensions = meta.external_dependencies_extensions
        });

        // Create MetaResource for each embedded resource
        for (const auto& embedded : meta.embeddedResources) {
            results.push_back({
                .uuid = embedded.uuid,
                .resourceType = embedded.resourceType,
                .format = embedded.format,
                .source = meta.uuid,  // Parent UUID
                .embeddedResources = {},
                .dependents = {},
                .external_dependencies_extensions = {}
            });
        }

        return results;
    }



    void ResourceIndexer::IndexMetaFile(const std::string &path)
    {
        auto resources = ParseMetaFile(path);
        for (auto& resource : resources) {
            m_resources[resource.uuid] = std::move(resource);
        }
    }


#ifdef SKAL_EDITOR
    void ResourceIndexer::RemoveDependent(const ResourceUUID resource, const ResourceUUID dependent)
    {
        if (!Exists(resource)) {
            Log::Warn("Cannot remove dependent: resource {} not found", resource.to_string());
            return;
        }

        auto& meta = m_resources[resource];

        if (!std::holds_alternative<std::string>(meta.source))
        {
            Log::Warn("ResourceIndexer::RemoveDependent, cant remove dependent since its not a source file");
            return;
        }

        const auto it = std::find(meta.dependents.begin(), meta.dependents.end(), dependent);

        if (it == meta.dependents.end())
        {
            Log::Info("ResourceIndexer::RemoveDependent, but resource : {} doesnt have {} as dependent", resource.to_string(), dependent.to_string());
            return;
        }

        meta.dependents.erase(it);



        const auto sourcePath = std::get<std::string>(meta.source);
        const auto metaPath = sourcePath + ".meta";

        const auto metaJson = SerializeMetaFile(meta);

        Engine.FileIO().WriteTextFile(metaPath, metaJson.dump(4));
    }

    void ResourceIndexer::AddDependent(const ResourceUUID resource, const ResourceUUID dependent)
    {
        if (!Exists(resource)) {
            Log::Warn("Cannot add dependent: resource {} not found", resource.to_string());
            return;
        }

        auto& meta = m_resources[resource];

        if (!std::holds_alternative<std::string>(meta.source))
        {
            Log::Warn("ResourceIndexer::AddDependent, cant add dependent since its not a source file");
            return;
        }


        meta.dependents.push_back(dependent);

        const auto sourcePath = std::get<std::string>(meta.source);
        const auto metaPath = sourcePath + ".meta";

        const auto metaJson = SerializeMetaFile(meta);

        Engine.FileIO().WriteTextFile(metaPath, metaJson.dump(4));
    }

    nlohmann::ordered_json ResourceIndexer::SerializeMetaFile(const MetaResource &meta)
    {
        nlohmann::ordered_json j;
        j["uuid"] = meta.uuid.to_string();
        j["type"] = ResourceTypeToString(meta.resourceType);
        j["format"] = meta.format;

        if (!meta.embeddedResources.empty()) {
            j["embedded"] = nlohmann::json::array();
            for (auto& e : meta.embeddedResources) {
                j["embedded"].push_back({
                    {"uuid", e.uuid.to_string()},
                    {"type", ResourceTypeToString(e.resourceType)},
                    {"format", e.format},
                    {"sub_path", e.subPath}
                });
            }
        }

        if (!meta.dependents.empty()) {
            j["dependents"] = nlohmann::json::array();
            for (auto& d : meta.dependents) {
                j["dependents"].push_back(d.to_string());
            }
        }

        if (!meta.external_dependencies_extensions.empty()) {
            j["external_dependencies_extensions"] = nlohmann::json::array();
            for (auto& ede : meta.external_dependencies_extensions) {
                j["external_dependencies_extensions"].push_back(ede);
            }
        }

        return j;
    }
#endif

} // skal