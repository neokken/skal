//
// Created by okker on 22/12/2025.
//

#include "skal/resource/resource_interface.h"


#ifdef SKAL_EDITOR
bool skal::IResource::Save()
{

    // Find path, call SaveToFile(), clear dirty
    m_dirty = false;
    return true;

}
#endif