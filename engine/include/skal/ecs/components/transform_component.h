//
// Created by okker on 22/12/2025.
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "skal/uuid_types.h"


namespace skal
{
    class Scene;

    struct Transform
    {
        glm::vec3 position{0.0f};
        glm::quat rotation{glm::identity<glm::quat>()};
        glm::vec3 scale{1.0f};

        [[nodiscard]] glm::mat4 GetLocalTransform() const;
        [[nodiscard]] const glm::mat4& GetWorldTransform() const;


        [[nodiscard]] glm::vec3 GetWorldPosition() const;
        [[nodiscard]] glm::quat GetWorldRotation() const;
        [[nodiscard]] glm::vec3 GetWorldScale() const;

        [[nodiscard]] bool HasParent() const { return !m_parent.is_null(); }
        [[nodiscard]] const EntityUUID& GetParent() const { return m_parent; }

        [[nodiscard]] const std::vector<EntityUUID>& GetChildren() const { return m_children; }

        [[nodiscard]] int GetDepth() const { return m_depth; }

    private:
        glm::mat4 m_cachedWorldTransform{1.0f};

        EntityUUID m_parent{};
        std::vector<EntityUUID> m_children{};
        int m_depth = 0;

        friend Scene;
    };

}
