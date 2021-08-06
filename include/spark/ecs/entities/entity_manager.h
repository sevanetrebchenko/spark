
#ifndef SPARK_ENTITY_MANAGER_H
#define SPARK_ENTITY_MANAGER_H

#include "spark/ecs/utility.h"
#include "spark/ecs/components/all_components.h"
#include "spark/ecs/components/component_manager_collection.h"

namespace Spark {
    namespace ECS {

        class EntityManager : public Singleton<EntityManager> {
            public:
                REGISTER_SINGLETON(EntityManager);

                void CreateEntity(const std::string& entityName);
                void DestroyEntity(EntityID ID);
                void DestroyEntity(const std::string& entityName);

                EntityID GetEntityIDFromName(const std::string& entityName) const;

                template <class ComponentType>
                void AddComponent(EntityID ID);

                template <class ComponentType>
                void RemoveComponent(EntityID ID);

                NODISCARD const EntityComponentMap* GetEntityComponentMap(const std::string& entityName) const;
                NODISCARD const EntityComponentMap* GetEntityComponentMap(EntityID entityID) const;

            private:
                EntityManager();

                EntityID GetNextEntityID();

                NODISCARD std::string GetValidEntityName(const std::string& entityName) const;

                template <class... ComponentTypes>
                NODISCARD bool EntityNameMatchesComponentName(const std::string& entityName) const;

                NODISCARD bool EntityNameMatchesEntityName(const std::string& entityName) const;

                EntityID entityIDCounter;
                std::unordered_map<EntityID, std::string> entityNames_{ };
                std::unordered_map<std::string, EntityID> entityIDs_{ };
                std::unordered_map<EntityID, EntityComponentMap> entityComponents_{ };
        };

    }
}

#include "spark/ecs/entities/entity_manager.tpp"

#endif // SPARK_ENTITY_MANAGER_H


