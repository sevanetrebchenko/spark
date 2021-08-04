
#ifndef SPARK_ENTITY_MANAGER_H
#define SPARK_ENTITY_MANAGER_H

#include <spark/core/rename.h>
#include <spark/ecs/components/types/base_component.h>          // BaseComponent
#include <spark/events/event_interactable_interface.h>          // IEventReceivable
#include <spark/ecs/components/all_components.h>                // COMPONENT_TYPES
#include "../../events/types/ecs_events.h"
#include "../utility.h"
#include "../components/component_manager_collection.h"
#include <spark/events/utility.h>

namespace Spark {
    namespace ECS {

        class EntityManager : public Singleton<EntityManager>,
                              UtilityBox::Logger::ILoggable,
                              REGISTER_EVENTS(EntityManager, WRAP(Events::AddComponentEvent, COMPONENT_TYPES), WRAP(Events::RemoveComponentEvent, COMPONENT_TYPES))
                              {
            public:
                REGISTER_SINGLETON(EntityManager);

                void CreateEntity(const std::string& entityName);
                void DestroyEntity(EntityID ID);
                void DestroyEntity(const std::string& entityName);

                template <class ComponentType>
                void AddComponent(EntityID ID);

                template <class ComponentType>
                void RemoveComponent(EntityID ID);

                NODISCARD const EntityComponentMap* GetEntityComponentMap(const std::string& entityName) const;
                NODISCARD const EntityComponentMap* GetEntityComponentMap(EntityID entityID) const;

            private:
                // Register AddComponentEvent/RemoveComponentEvent event function overrides for all registered components.
                REGISTER_COMPONENT_TYPE_OVERRIDES(COMPONENT_TYPES);

                NODISCARD std::string GetValidEntityName(const std::string& entityName) const;

                template <class... ComponentTypes>
                NODISCARD bool EntityNameMatchesComponentName(const std::string& entityName) const;

                NODISCARD bool EntityNameMatchesEntityName(const std::string& entityName) const;

                std::unordered_map<EntityID, std::string> entityNames_{ };
                std::unordered_map<EntityID, EntityComponentMap> entityComponents_{ };
        };

    }
}

#include <spark/ecs/entities/entity_manager.tpp>

#endif // SPARK_ENTITY_MANAGER_H


