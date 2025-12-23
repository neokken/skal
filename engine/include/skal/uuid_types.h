//
// Created by okker on 21/12/2025.
//

#pragma once

#include "skal/util/uuid.h"

namespace skal
{
    struct ResourceTag {};
    using ResourceUUID = TypedUUID<ResourceTag>;

    struct EntityTag {};
    using EntityUUID = TypedUUID<EntityTag>;
}