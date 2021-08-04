
#ifndef SPARK_BASE_COMPONENT_SYSTEM_H
#define SPARK_BASE_COMPONENT_SYSTEM_H

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/logging_interface.h>   // ILoggable
#include <spark/ecs/systems/base_component_system_interface.h> // IBaseComponentSystem

#include <spark/events/event_interactable_interface.h>
#include <spark/events/types/ecs_events.h>
#include <spark/ecs/components/utility.h>

namespace Spark {
    namespace ECS {

        template <class ...ComponentTypes>
        class BaseComponentSystem : public IBaseComponentSystem,
                                    UtilityBox::Logger::ILoggable,
                                    REGISTER_EVENTS(BaseComponentSystem<ComponentTypes...>, Events::DestroyEntityEvent, Events::AddComponentSystemConfigEvent, Events::RemoveComponentSystemConfigEvent)
                                    {
            public:
                explicit BaseComponentSystem(const std::string& systemName = std::string("Component System (" + Internal::CommaSeparatedList<ComponentTypes...>() + ")"));
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
                void OnEvent(Events::DestroyEntityEvent* event) override;
                void OnEvent(Events::AddComponentSystemConfigEvent* event) override;
                void OnEvent(Events::RemoveComponentSystemConfigEvent* event) override;

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
                bool ProcessEntityComponent(ComponentTypeID componentTypeID, BaseComponent* component, ComponentTuple& componentTuple);

                template <unsigned>
                bool ProcessEntityComponent(ComponentTypeID, BaseComponent*, ComponentTuple&);

                // Mapping relationships between Entity ID and index in entities_ vector.
                std::unordered_map<EntityID, unsigned> entityIDToIndex_;
                std::unordered_map<unsigned, EntityID> indexToEntityID_;
        };

    }
}

#include <spark/ecs/systems/base_component_system.tpp> // Template function definitions.

#endif // SPARK_BASE_COMPONENT_SYSTEM_H

