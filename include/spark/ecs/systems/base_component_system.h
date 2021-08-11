
#ifndef SPARK_BASE_COMPONENT_SYSTEM_H
#define SPARK_BASE_COMPONENT_SYSTEM_H

#include "spark/core/utility.h"
#include "spark/ecs/systems/base_component_system_interface.h"
#include "spark/events/types/ecs_events.h"
#include "spark/events/event_listener.h"

namespace Spark {
    namespace ECS {

        template <class ...ComponentTypes>
        class BaseComponentSystem : public IBaseComponentSystem,
                                    Events::EventListener<BaseComponentSystem<ComponentTypes...>, Events::EntityCreatedEvent, Events::EntityDestroyedEvent, Events::RefreshObjectComponentListEvent>
                                    {
            public:
                BaseComponentSystem();
                ~BaseComponentSystem();

                void Initialize() override;
                void Update(float dt) override;
                void Shutdown() override;

            protected:
                using BaseSystem = BaseComponentSystem<ComponentTypes...>;
                using ComponentTuple = std::tuple<ComponentTypes*...>;

                template <typename ComponentType>
                ComponentType* GetComponent(unsigned index);

                template <typename ComponentType>
                ComponentType* GetComponent(const ComponentTuple& componentTuple);

                std::vector<ComponentTuple> tuples_; // Tuples managed by this system.

            private:
                void OnEvent(Events::EntityCreatedEvent* event) override;
                void OnEvent(Events::EntityDestroyedEvent* event) override;
                void OnEvent(Events::RefreshObjectComponentListEvent* event) override;

                // Used to retrieve a specific type of component from a given ComponentTuple, given that it exists.
                template <class DesiredComponentType, unsigned Index, class ComponentType, class... AdditionalComponentTypes>
                DesiredComponentType* GetComponentHelper(const ComponentTuple& componentTuple) const;

                template <class DesiredComponentType, unsigned Index>
                DesiredComponentType* GetComponentHelper(const ComponentTuple& componentTuple) const;

                bool FilterEntity(const EntityComponentMap& entityComponents, ComponentTuple& tuple) const;
                void RemoveEntity(EntityID entityID);

                void InsertTuple(EntityID entityID, ComponentTuple& tuple);

                // Used to check if a given component type
                template <unsigned Index, class ComponentType, class ...AdditionalComponentTypes>
                bool ProcessEntityComponent(ComponentTypeID componentTypeID, IComponent* component, ComponentTuple& componentTuple);

                template <unsigned>
                bool ProcessEntityComponent(ComponentTypeID, IComponent*, ComponentTuple&);

                // Mapping relationships between Entity ID and index in entities_ vector.
                std::unordered_map<EntityID, unsigned> entityIDToIndex_;
                std::unordered_map<unsigned, EntityID> indexToEntityID_;
        };

    }
}

#include <spark/ecs/systems/base_component_system.tpp> // Template function definitions.

#endif // SPARK_BASE_COMPONENT_SYSTEM_H

