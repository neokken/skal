//
// Created by okker on 22/12/2025.
//

#pragma once
#include "skal/uuid_types.h"

namespace skal
{
    class IResource;

    class RefCountManager
    {
        static void IncrementRef(ResourceUUID uuid);
        static void DecrementRef(ResourceUUID uuid);

        static IResource* GetResource(ResourceUUID uuid);

        template <class Resource>
        friend class Handle;
    };

    template<class Resource>
    class Handle
    {
    public:
        Handle() = default;
        ~Handle()
        {
            RefCountManager::DecrementRef(GetUUID());
        }
        Handle(const Handle& other)
        {
            m_resourceUUID = other.m_resourceUUID;
            RefCountManager::IncrementRef(GetUUID());
        }
        Handle(Handle&& other) noexcept
        {
            m_resourceUUID = other.m_resourceUUID;
            other.m_resourceUUID = ResourceUUID::null();
        }
        Handle& operator=(const Handle& other)
        {
            if (this != &other)
            {
                RefCountManager::DecrementRef(GetUUID());

                m_resourceUUID = other.m_resourceUUID;
                RefCountManager::IncrementRef(GetUUID());
            }
            return *this;
        }
        Handle& operator=(Handle&& other) noexcept
        {
            if (this != &other)
            {
                RefCountManager::DecrementRef(GetUUID());
                m_resourceUUID = other.m_resourceUUID;
                other.m_resourceUUID = ResourceUUID::null();
            }
            return *this;
        }


        [[nodiscard]] const ResourceUUID &GetUUID() const { return m_resourceUUID; }
        [[nodiscard]] bool IsValid() const { return !m_resourceUUID.is_null(); }

        [[nodiscard]] const Resource *Get() const
        {
            return static_cast<Resource *>(RefCountManager::GetResource(m_resourceUUID));
        }

        [[nodiscard]] Resource *Get()
        {
            return static_cast<Resource *>(RefCountManager::GetResource(m_resourceUUID));
        }

    private:
        explicit Handle(const ResourceUUID &uuid) : m_resourceUUID(uuid)
        {
            RefCountManager::IncrementRef(uuid);
        }

        friend class ResourceManager;
        ResourceUUID m_resourceUUID;
    };
}
