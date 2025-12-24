//
// Created by okker on 23/12/2025.
//

#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>

namespace skal
{

    template<typename T>
    class ResourcePool
    {
    public:
        uint32_t Allocate(T &&resource)
        {
            if (!m_freeList.empty())
            {
                uint32_t id = m_freeList.back();
                m_freeList.pop_back();
                m_resources[id] = std::move(resource);
                return id;
            }

            m_resources.push_back(std::move(resource));
            return static_cast<uint32_t>(m_resources.size());
        }

        void Free(uint32_t handle)
        {
            if (handle == 0 || handle > m_resources.size()) return;
            m_resources[handle - 1] = T{};
            m_freeList.push_back(handle);
        }

        T* Get(uint32_t handle)
        {
            if (handle == 0 || handle > m_resources.size()) return nullptr;
            return &m_resources[handle - 1];
        }

    private:
        std::vector<T> m_resources;
        std::vector<uint32_t> m_freeList;
    };
}
