//
// Created by okker on 22/12/2025.
//

#include "skal/ecs/components/transform_component.h"

#include "glm/gtx/matrix_decompose.hpp"

glm::mat4 skal::Transform::GetLocalTransform() const
{
    auto transform = glm::identity<glm::mat4>();
    transform = glm::translate(transform, position);
    transform *= glm::mat4_cast(rotation);
    transform = glm::scale(transform, scale);
    return transform;
}

const glm::mat4 & skal::Transform::GetWorldTransform() const
{
    return m_cachedWorldTransform;
}

glm::vec3 skal::Transform::GetWorldPosition() const
{
    return {m_cachedWorldTransform[3]};
}

glm::quat skal::Transform::GetWorldRotation() const
{
    glm::vec3 tempScale, tempTranslation, tempSkew;
    glm::vec4 tempPerspective;
    glm::quat tempRotation;
    glm::decompose(m_cachedWorldTransform, tempScale, tempRotation, tempTranslation, tempSkew, tempPerspective);
    return tempRotation;
}

glm::vec3 skal::Transform::GetWorldScale() const
{
    return {glm::length(glm::vec3(m_cachedWorldTransform[0])),
        glm::length(glm::vec3(m_cachedWorldTransform[1])),
        glm::length(glm::vec3(m_cachedWorldTransform[2]))};
}
