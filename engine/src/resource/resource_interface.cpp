//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_interface.h"

#include "skal/engine.h"
#include "skal/resource/resource_indexer.h"


#ifdef SKAL_EDITOR
bool skal::IResource::Save()
{

    const auto resourceSource = skal::Engine.ResourceIndexer().Get(GetUUID())->source;

    if (std::holds_alternative<PakLocation>(resourceSource))
    {
        Log::Error("IResource::Save - Trying to save a resource, but its a packaged resource.");
        return false;
    }
    else if (std::holds_alternative<ResourceUUID>(resourceSource))
    {
        Log::Error("IResource::Save - Trying to save a resource, but its a embedded resource.");
        return false;
    }

    SaveToFile(std::get<std::string>(resourceSource));

    // Find path, call SaveToFile(), clear dirty
    m_dirty = false;
    return true;

}
#endif