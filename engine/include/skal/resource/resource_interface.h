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
    };

}
