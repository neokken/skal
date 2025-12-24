//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_manager.h"

#include "skal/file_io.h"
#include "skal/resource/resource_interface.h"
#include "skal/util/log.h"

namespace skal
{
    void ResourceManager::Unload(const ResourceUUID uuid)
    {
        const auto it = m_resources.find(uuid);
        if (it == m_resources.end())
        {
            skal::Log::Error("ResourceManager::Unload - Tried to unload {}, while it was not loaded", uuid.to_string());
            return;
        }

        m_resources.erase(it);
        skal::Log::Info("Unloaded resource: {}", uuid.to_string());
    }

    void ResourceManager::GarbageCollect()
    {
        bool removedAny = false;
        bool changed = true;

        while (changed)
        {
            changed = false;
            std::vector<ResourceUUID> toRemove;

            for (const auto &[uuid, entry]: m_resources)
            {
                if (entry.refCount == 0)
                {
                    toRemove.push_back(uuid);
                }
            }

            if (!toRemove.empty())
            {
                skal::Log::Info("GarbageCollected {} resources, unloading", toRemove.size());
                changed = true;
                removedAny = true;

                for (const auto &uuid: toRemove)
                {
                    m_resources.erase(uuid);
                }
            }
        }

        if (removedAny)
        {
            Log::Info("Garbage collection complete");
        }
    }

    IResource* ResourceManager::LoadFromDisk(const MetaResource &meta)
    {
        // Load data based on source type

        if (std::holds_alternative<std::string>(meta.source))
        {
            // Load from file
            const auto path = std::get<std::string>(meta.source);
            const std::vector<uint8_t> data = Engine.FileIO().ReadBinaryFile(path);


            return ResourceFactory::Create(meta.resourceType, meta.uuid, meta.format, data, path);
        }
        else if (std::holds_alternative<ResourceUUID>(meta.source))
        {
            // Embedded resource - load parent first
            const auto parentUUID = std::get<ResourceUUID>(meta.source);

            // Recursively load parent (which creates embedded resources)
            Load<IResource>(parentUUID);

            // Parent's constructor created this embedded resource
            // Return it from cache
            if (m_resources.find(meta.uuid) != m_resources.end())
            {
                return m_resources[meta.uuid].resource.get();
            }

            Log::Error("ResourceManager::LoadFromDisk - Parent loaded but embedded resource not found: {}", meta.uuid.to_string());
            return nullptr;
        }
        else if (std::holds_alternative<PakLocation>(meta.source))
        {
            // Load from pak
            return nullptr;
        }

        return nullptr;
    }

    IResource* ResourceManager::GetRaw(const ResourceUUID uuid)
    {
        const auto it = m_resources.find(uuid);
        if (it == m_resources.end())
        {
            return nullptr;
        }

        return it->second.resource.get();
    }

    void ResourceManager::IncrementRef(const ResourceUUID uuid)
    {
        if (uuid.is_null()) return;

        const auto it = m_resources.find(uuid);
        if (it == m_resources.end())
        {
            Log::Error("ResourceManager::IncrementRef - resource doesn't exist: {}", uuid.to_string());
            return;
        }

        it->second.refCount++;
    }

    void ResourceManager::DecrementRef(const ResourceUUID uuid)
    {
        if (uuid.is_null()) return;

        const auto it = m_resources.find(uuid);
        if (it == m_resources.end())
        {
            Log::Error("ResourceManager::DecrementRef - resource doesn't exist: {}", uuid.to_string());
            return;
        }

        it->second.refCount--;

        if (it->second.refCount < 0)
        {
            Log::Error("ResourceManager::DecrementRef - Negative refcount for resource: {}", uuid.to_string());
            it->second.refCount = 0;
        }
    }
} // skal
