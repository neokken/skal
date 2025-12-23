//
// Created by okker on 22/12/2025.
//

#pragma once
#include "nlohmann/json_fwd.hpp"

// warning suppressors maybe needed?
#include <entt/entt.hpp>

#include "skal/uuid_types.h"


namespace skal
{
    using Entity = entt::entity;


    class Scene
    {
    public:
        Scene();

        Scene(Scene &) = delete;
        Scene(Scene &&) = delete;

        Scene &operator=(Scene &) = delete;
        Scene &operator=(Scene &&) = delete;


        // ======== UPDATE ========

        void Update();


        // ===== PRIMARY API: UUID-based,so should be "safer") =====
        EntityUUID CreateEntity(const std::string &name= "default");
        void DestroyEntity(EntityUUID uuid);
        void SetParent(EntityUUID child, EntityUUID parent, int position = -1);

        // Conversion
        Entity GetEntity(EntityUUID uuid) const;
        EntityUUID GetUUID(Entity entity) const;

        Entity GetRoot() const {return m_rootEntity; }
        EntityUUID GetRootUUID() const {return GetUUID(m_rootEntity); }


        // ===== INTERNAL: Entity-based =====
        Entity CreateEntityInternal(const std::string &name= "default");
        void DestroyEntityInternal(Entity entity);


        // ======== SERIALIZATION ========

        void Deserialize(const nlohmann::json &scene);
        nlohmann::ordered_json Serialize();

        // ======== WRAPPERS ========

        bool IsValid(Entity entity) const;

        template<typename Type, typename... Args>
        Type &AddComponent(const Entity entity, Args &&... args)
        {
            return m_registry.emplace<Type>(entity, std::forward<Args>(args)...);
        }

        template<typename Type>
        Type &AddComponent(const Entity entity)
        {
            return m_registry.emplace<Type>(entity);
        }

        template<typename Type, typename... Other>
        void RemoveComponent(const Entity entity)
        {
            m_registry.remove<Type, Other...>(entity);
        }

        template<typename Type>
        Type &GetComponent(const Entity entity)
        {
            return m_registry.get<Type>(entity);
        }

        template<typename Type>
        const Type &GetComponent(const Entity entity) const
        {
            return m_registry.get<Type>(entity);
        }

        template<typename Type>
        bool HasComponent(const Entity entity) const
        {
            return m_registry.all_of<Type>(entity);
        }

        template<typename... Components>
        bool HasAllComponents(const Entity entity) const
        {
            return m_registry.all_of<Components...>(entity);
        }

        template<typename... Components>
        bool HasAnyComponent(const Entity entity) const
        {
            return m_registry.any_of<Components...>(entity);
        }

        template<typename Type>
        Type *TryGetComponent(const Entity entity)
        {
            return m_registry.try_get<Type>(entity);
        }

        template<typename Type>
        const Type *TryGetComponent(const Entity entity) const
        {
            return m_registry.try_get<Type>(entity);
        }

        template<typename Type, typename... Other, typename... Exclude>
        auto GetView(entt::exclude_t<Exclude...> exclude = entt::exclude_t<Exclude...>{})
        {
            return m_registry.view<Type, Other...>(exclude);
        }

        template<typename Type, typename... Other, typename... Exclude>
        auto GetView(entt::exclude_t<Exclude...> exclude = entt::exclude_t<Exclude...>{}) const
        {
            return m_registry.view<Type, Other...>(exclude);
        }


    private:
        void UpdateDepthsRecursive(Entity entity, int depth);

        void CleanupDeletedEntities();

        void SortHierarchyByDepth();
        void CalculateHierarchy();

    private:
        entt::registry m_registry;

        std::unordered_set<Entity> m_entitiesToBeDeleted;
        std::unordered_map<EntityUUID, Entity> m_uuidToEntity;
        std::unordered_map<Entity, EntityUUID> m_entityToUuid;

        Entity m_rootEntity{entt::null};

        bool m_hierarchyDirty{false};
    };
} // skal
