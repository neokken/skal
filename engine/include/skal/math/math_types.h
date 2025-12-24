//
// Created by okker on 24/12/2025.
//


#pragma once
#include "glm/vec3.hpp"

namespace skal
{

    struct AABB  // Axis-Aligned Bounding Box
    {
        glm::vec3 min{0};
        glm::vec3 max{0};
    };

    struct Sphere
    {
        glm::vec3 center{0};
        float radius{0};
    };

    struct Plane
    {
        glm::vec3 normal{0, 1, 0};
        float distance{0};
    };

    struct Ray
    {
        glm::vec3 origin{0};
        glm::vec3 direction{0, 0, 1};
    };


}
