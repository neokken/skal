//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_handle.h"

#include "skal/engine.h"
#include "skal/resource/resource_manager.h"

namespace skal
{
    void RefCountManager::IncrementRef(const ResourceUUID uuid)
    {
        Engine.ResourceManager().IncrementRef(uuid);
    }

    void RefCountManager::DecrementRef(const ResourceUUID uuid)
    {
        Engine.ResourceManager().DecrementRef(uuid);
    }

    IResource* RefCountManager::GetResource(const ResourceUUID uuid)
    {
        return Engine.ResourceManager().GetRaw(uuid);
    }
}
