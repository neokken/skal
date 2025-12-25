//
// Created by okker on 22/12/2025.
//

#pragma once
#include "skal/resource/resource_types.h"
#include "skal/uuid_types.h"
#include "skal/util/log.h"


namespace skal
{
    struct ImportedAsset;

    class IResource
    {
    public:
        virtual ~IResource() = default;

        [[nodiscard]] virtual ResourceType GetType() const = 0;
        [[nodiscard]] const ResourceUUID& GetUUID() const { return m_uuid; }

        virtual void Load(const ImportedAsset& asset) = 0;

    protected:
        explicit IResource(const ResourceUUID uuid) : m_uuid(uuid) {}


    private:
        ResourceUUID m_uuid;

        // ========== Editor-only functionality ==========

#ifdef SKAL_EDITOR
        virtual bool SaveToFile(const std::string& path) { skal::Log::Warn("SaveToFile has not been implemented for this resource."); return false; }

    public:
        void MarkDirty() { m_dirty = true; }
        bool Save();    // will save to the location its stored.
        [[nodiscard]] bool IsDirty() const { return m_dirty; }

    private:
        bool m_dirty{false};
#endif
    };

}
