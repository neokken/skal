//
// Created by okker on 22/12/2025.
//

#include "skal/ecs/scene.h"

#include "nlohmann/json.hpp"
#include "skal/ecs/components/engine_component_tags.h"
#include "skal/ecs/components/transform_component.h"
#include "skal/ecs/components/uuid_component.h"
#include "skal/util/log.h"

namespace skal
{
    Scene::Scene()
    {
        m_rootEntity = CreateEntityInternal("root");
        AddComponent<RootTag>(m_rootEntity);
    }

    void Scene::UpdateDepthsRecursive(const Entity entity, const int depth)
    {
        auto &transform = GetComponent<Transform>(entity);
        transform.m_depth = depth;

        for (const EntityUUID childUUID: transform.m_children)
        {
            const Entity childEntity = GetEntity(childUUID);
            UpdateDepthsRecursive(childEntity, depth + 1);
        }
    }

    void Scene::CleanupDeletedEntities()
    {
        if (m_entitiesToBeDeleted.empty()) return;

        while (!m_entitiesToBeDeleted.empty())
        {
            Entity entity = *m_entitiesToBeDeleted.begin();
            m_entitiesToBeDeleted.erase(entity);

            if (!m_registry.valid(entity)) continue;

            const auto* transform = TryGetComponent<Transform>(entity);
            if (transform && transform->HasParent())
            {
                const Entity parentEntity = GetEntity(transform->GetParent());
                if (parentEntity != entt::null)
                {
                    auto &parentTransform = GetComponent<Transform>(parentEntity);
                    EntityUUID thisUUID = GetUUID(entity);

                    auto it = std::find(parentTransform.m_children.begin(),
                                        parentTransform.m_children.end(),
                                        thisUUID);
                    if (it != parentTransform.m_children.end())
                    {
                        parentTransform.m_children.erase(it);
                    }
                }
            }

            EntityUUID uuid = GetUUID(entity);
            m_entityToUuid.erase(entity);
            m_uuidToEntity.erase(uuid);

            m_registry.destroy(entity);
        }

        m_hierarchyDirty = true;
    }

    void Scene::SortHierarchyByDepth()
    {
        if (!m_hierarchyDirty) return;
        m_hierarchyDirty = false;

        UpdateDepthsRecursive(m_rootEntity, 0);

        m_registry.sort<Transform>([](const Transform &lhs, const Transform &rhs)
        {
            return lhs.m_depth < rhs.m_depth;
        });
    }

    void Scene::CalculateHierarchy()
    {
        const auto view = m_registry.view<Transform>();
        for (const auto entity : view)
        {
            auto& transform = view.get<Transform>(entity);

            if (transform.HasParent())
            {
                const Entity parent_entity = GetEntity(transform.GetParent());


                if (parent_entity == entt::null) // Parent is already deleted
                {
                    DestroyEntityInternal(entity);
                    continue;
                }

                if (HasComponent<MarkedForDeleteTag>(parent_entity))
                {
                    DestroyEntityInternal(entity);
                    continue;
                }

                const auto& parent_transform = GetComponent<Transform>(parent_entity);
                transform.m_cachedWorldTransform = parent_transform.m_cachedWorldTransform * transform.GetLocalTransform();
            }
            else
            {
                transform.m_cachedWorldTransform = transform.GetLocalTransform();
            }
        }
    }

    void Scene::Update()
    {
        SortHierarchyByDepth();

        CalculateHierarchy();
        CleanupDeletedEntities();
    }

    EntityUUID Scene::CreateEntity(const std::string &name)
    {
        return GetUUID(CreateEntityInternal(name));
    }

    void Scene::DestroyEntity(EntityUUID uuid)
    {
        const auto e = GetEntity(uuid);
        DestroyEntityInternal(e);
    }

    void Scene::SetParent(const EntityUUID child, const EntityUUID parent, int position)
    {
        const Entity childEntity = GetEntity(child);
        if (childEntity == entt::null)
        {
            Log::Error("Scene::SetParent - Invalid child UUID");
            return;
        }

        // Cannot reparent root
        if (HasComponent<RootTag>(childEntity))
        {
            Log::Error("Scene::SetParent - Cannot reparent root entity");
            return;
        }

        // Null parent not allowed - must use root
        EntityUUID actualParent = parent;
        if (actualParent == EntityUUID::null())
        {
            Log::Warn("Scene::SetParent - Null parent not allowed, using scene root");
            //TODO (okke): can be probably be removed since its quite nice to just set the parent as nothing?
            actualParent = GetRootUUID();
        }

        const Entity parentEntity = GetEntity(actualParent);
        if (parentEntity == entt::null)
        {
            Log::Error("Scene::SetParent - Invalid parent UUID");
            return;
        }

        // cycle check
        EntityUUID checkUUID = actualParent;
        int safetyCounter = 0;
        constexpr int maxDepth = 1000;
        while (checkUUID != EntityUUID::null())
        {
            if (++safetyCounter > maxDepth)
            {
                Log::Error("Scene::SetParent - Hierarchy traversal exceeded maximum depth, possible existing cycle");
                return;
            }

            if (checkUUID == child)
            {
                Log::Error("Scene::SetParent - Cannot parent entity to its own descendant, would create cycle");
                return;
            }

            const Entity checkEntity = GetEntity(checkUUID);
            const auto &checkTransform = GetComponent<Transform>(checkEntity);
            checkUUID = checkTransform.GetParent();

            // Safety: detect if we somehow don't reach root
            if (HasComponent<RootTag>(checkEntity))
            {
                break; // Reached root, no cycle
            }
        }


        auto &childTransform = GetComponent<Transform>(childEntity);
        const EntityUUID oldParentUUID = childTransform.GetParent();

        const bool isSameParent = (oldParentUUID == parent);
        int oldPosition = -1;

        // Remove from old parent
        if (childTransform.HasParent())
        {
            const Entity oldParentEntity = GetEntity(oldParentUUID);
            auto &oldParentHierarchy = GetComponent<Transform>(oldParentEntity);
            const auto it = std::find(oldParentHierarchy.m_children.begin(), oldParentHierarchy.m_children.end(),
                                      child);
            if (it != oldParentHierarchy.m_children.end())
            {
                if (isSameParent)
                {
                    oldPosition = static_cast<int>(it - oldParentHierarchy.m_children.begin());
                }
                oldParentHierarchy.m_children.erase(it);
            }
        }

        // Adjust position if moving within same parent and old position was before new position
        if (isSameParent && oldPosition != -1 && oldPosition < position)
        {
            position--;
        }


        if (parentEntity != entt::null)
        {
            auto &parentTransform = GetComponent<Transform>(parentEntity);
            childTransform.m_parent = parent;
            if (position < 0 || position >= static_cast<int>(parentTransform.m_children.size()))
            {
                parentTransform.m_children.push_back(child);
            } else
            {
                parentTransform.m_children.insert(parentTransform.m_children.begin() + position, child);
            }
        }


        m_hierarchyDirty = true;
    }

    Entity Scene::GetEntity(const EntityUUID uuid) const
    {
        const auto it = m_uuidToEntity.find(uuid);
        return it != m_uuidToEntity.end() ? it->second : entt::null;
    }

    EntityUUID Scene::GetUUID(const Entity entity) const
    {
        const auto it = m_entityToUuid.find(entity);
        return it != m_entityToUuid.end() ? it->second : EntityUUID{};
    }

    Entity Scene::CreateEntityInternal(const std::string &name)
    {
        const auto uuid = skal::UUIDGenerator::generate<EntityUUID>();
        const auto entity = m_registry.create();
        auto &uuidComp = AddComponent<UUIDComponent>(entity);
        uuidComp.uuid = uuid;

        m_uuidToEntity[uuid] = entity;
        m_entityToUuid[entity] = uuid;

        AddComponent<Transform>(entity);
        AddComponent<NameComponent>(entity).name = name;

        if (m_rootEntity != entt::null)
            SetParent(uuid, GetRootUUID());

        m_hierarchyDirty = true;
        return entity;
    }

    void Scene::DestroyEntityInternal(const Entity entity)
    {
        if (HasComponent<RootTag>(entity))
        {
            skal::Log::Error("Scene::DestroyEntityInternal - Attempted to destroy root entity");
            return;
        }

        m_entitiesToBeDeleted.insert(entity);
        if (!HasComponent<MarkedForDeleteTag>(entity))
            AddComponent<MarkedForDeleteTag>(entity);
    }

    nlohmann::ordered_json Scene::Serialize()
    {
        nlohmann::ordered_json j;
        j["scene"] = "data";
        return j;   //TODO (okke): serialization
    }

    void Scene::Deserialize([[maybe_unused]] const nlohmann::json &scene)    //TODO (okke): deserialization: [[maybe unused]]
    {
        const auto view = m_registry.view<UUIDComponent>();
        std::vector<Entity> toDelete;

        for (const Entity e : view) {
            if (!HasComponent<RootTag>(e)) {
                toDelete.push_back(e);
            }
        }

        for (const Entity e : toDelete) {
            DestroyEntityInternal(e);
        }

        CleanupDeletedEntities();

        const EntityUUID rootUUID = GetUUID(m_rootEntity);
        m_uuidToEntity.clear();
        m_entityToUuid.clear();

        m_uuidToEntity[rootUUID] = m_rootEntity;
        m_entityToUuid[m_rootEntity] = rootUUID;

        m_hierarchyDirty = true;
    }

    bool Scene::IsValid(const Entity entity) const
    {
        return m_registry.valid(entity) && m_entitiesToBeDeleted.find(entity) == m_entitiesToBeDeleted.end();
    }
} // skal
