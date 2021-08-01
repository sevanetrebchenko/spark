
#ifndef SPARK_ENTITY_MANAGER_H
#define SPARK_ENTITY_MANAGER_H

#include <spark/core/rename.h>
#include <spark/ecs/entities/entity_callback_type.h>            // CallbackType
#include <spark/ecs/ecs_typedefs.h>                             // EntityID
#include <spark/ecs/components/types/base_component.h>          // BaseComponent
#include <spark/ecs/entities/entity_manager_interface.h>        // IEntityManager
#include <spark/events/event_interactable_interface.h>          // IEventReceivable
#include <spark/ecs/components/all_components.h>                // ALL_COMPONENTS
#include "../../events/types/ecs_events.h"

namespace Spark {
    namespace ECS {

        class EntityManager : public IEntityManager, REGISTER_EVENTS(EntityManager, WRAP(Events::AddComponentEvent, ALL_COMPONENTS), WRAP(Events::RemoveComponentEvent, ALL_COMPONENTS)) {
            public:
                static EntityManager* GetInstance();

                void CreateEntity(std::string name) override;
                void DestroyEntity(EntityID ID) override;
                void DestroyEntity(std::string name) override;

                NODISCARD EntityID GetEntityIDFromName(const std::string& entityName) const override;

                // Entity manager should not be copied/duplicated.
                EntityManager(EntityManager const &other) = delete;
                EntityManager(EntityManager &&other) = delete;
                void operator=(EntityManager const &other) = delete;

            private:
                EntityManager();
                ~EntityManager();
                static EntityManager* s_instance;

                // Events from event system.
                void OnEvent(Events::AddComponentEvent<TestComponent>* event) override;
                void OnEvent(Events::RemoveComponentEvent<TestComponent>* event) override;

                template <class ComponentType>
                void AddComponent(EntityID ID);

                template <class ComponentType>
                void RemoveComponent(EntityID ID);

                template <class ...ComponentTypes>
                bool CheckEntityName(const std::string& name) const;

                std::unordered_map<EntityID, std::string> _entityNames;              // Lsit of entity names.
                std::unordered_map<EntityID, EntityComponentMap> _entityComponents; // List of components attached to entities.
        };

    }
}

#endif // SPARK_ENTITY_MANAGER_H


