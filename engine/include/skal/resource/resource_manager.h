//
// Created by okker on 22/12/2025.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "resource_handle.h"
#include "resource_indexer.h"
#include "skal/engine.h"
#include "skal/uuid_types.h"

namespace skal
{
    struct MetaResource;
    class IResource;


    class ResourceManager
    {
    public:
        template<typename T>
        Handle<T> Load(ResourceUUID uuid)
        {
            if (uuid.is_null())
            {
                return Handle<T>();
            }

            // Return cached if already loaded
            if (m_resources.find(uuid) != m_resources.end())
            {
                return Handle<T>(uuid);
            }

            auto* meta = skal::Engine.ResourceIndexer().Get(uuid);
            if (meta == nullptr)
            {
                Log::Error("ResourceManager::Load - No indexed file with UUID: {}", uuid.to_string());
                return Handle<T>();
            }

            IResource *resource = LoadFromMeta(*meta);

            if (!resource)
            {
                Log::Error("ResourceManager::Load - Failed to load resource: {}", uuid.to_string());
                return Handle<T>();
            }

            if (dynamic_cast<T*>(resource) == nullptr)
            {
                Log::Error("ResourceManager::Load - Loaded resource type mismatch for UUID: {}", uuid.to_string());
                delete resource;
                return Handle<T>();
            }

            m_resources[uuid] = ResourceEntry{std::unique_ptr<IResource>(resource), 0};
            return Handle<T>(uuid);
        }

        template<typename T>
        Handle<T> Create(const ImportedAsset& asset)
        {
            auto uuid = UUIDGenerator::generate<ResourceUUID>();

            auto resource = std::make_unique<T>(uuid);
            resource->Load(asset);
            m_resources[uuid] = ResourceEntry{std::move(resource), 0};

            return Handle<T>(uuid);
        }

        void Unload(ResourceUUID uuid);

        void GarbageCollect(); // Unload resources with refCount == 0

    private:
        struct ResourceEntry
        {
            std::unique_ptr<IResource> resource;
            int refCount = 0;
        };

        std::unordered_map<ResourceUUID, ResourceEntry> m_resources;

        static IResource* LoadFromMeta(const MetaResource &meta);

        IResource* GetRaw(ResourceUUID uuid);

        void IncrementRef(ResourceUUID uuid);

        void DecrementRef(ResourceUUID uuid);

        friend class RefCountManager;
    };
} // skal
