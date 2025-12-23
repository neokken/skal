//
// Created by okker on 22/12/2025.
//

#pragma once
#include "skal/uuid_types.h"

namespace skal
{
    struct UUIDComponent
    {
        EntityUUID Get() const { return uuid; }

    private:
        EntityUUID uuid;
        friend class Scene;
    };
}
