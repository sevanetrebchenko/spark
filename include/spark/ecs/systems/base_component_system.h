
#ifndef SPARK_BASE_COMPONENT_SYSTEM_H
#define SPARK_BASE_COMPONENT_SYSTEM_H

#include <spark/core/core.h>
#include <spark/utilitybox/logger/logging_interface.h>   // ILoggable
#include <spark/ecs/systems/base_component_system_interface.h> // IBaseComponentSystem

namespace Spark {
    namespace ECS {
        namespace Systems {

            template <class ...ComponentTypes>
            class BaseComponentSystem : public IBaseComponentSystem, UtilityBox::Logger::ILoggable {
                public:
                    explicit BaseComponentSystem(const std::string& systemName);
                    ~BaseComponentSystem();

                    void Initialize() override;
                    void Update(float dt) override;
                    void Shutdown() override;

                protected:
                    using BaseSystem = BaseComponentSystem<ComponentTypes...>;
                    using ComponentTuple = std::tuple<ComponentTypes*...>; // Tuple of component pointers.

                    template <typename ComponentType>
                    ComponentType* GetComponent(unsigned index);

                    template <typename ComponentType>
                    ComponentType* GetComponent(const ComponentTuple& componentTuple);

                    std::vector<ComponentTuple> _filteredEntities; // Array of component tuples of entities managed by this system.

                private:
                    void OnEntityCreate(EntityID ID);
                    void OnEntityDestroy(EntityID ID);
                    void OnEntityComponentAdd(EntityID ID);
                    void OnEntityComponentRemove(EntityID ID);

                    template <class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
                    DesiredComponentType* GetComponentHelper(const ComponentTuple& componentTuple);

                    void VerifyEntity(EntityID ID);
                    std::pair<bool, ComponentTuple> FilterEntity(ECS::EntityID ID);
                    void RemoveEntity(const std::unordered_map<EntityID, unsigned>::const_iterator& entityToDeleteIterator);

                    template <unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
                    bool ProcessEntityComponent(ComponentTypeID componentTypeID, Components::BaseComponent* component, ComponentTuple& componentTuple);

                    template <unsigned>
                    bool ProcessEntityComponent(ComponentTypeID, Components::BaseComponent*, ComponentTuple&);

                    std::unordered_map<EntityID, unsigned> _entityIDToContainerIndex; // Mapping from entity ID to index in filtered entities vector.
                    std::unordered_map<unsigned, EntityID> _containerIndexToEntityID; // Mapping from index in filtered entities vector to entity ID.
            };

        }
    }
}

#include <spark/ecs/systems/base_component_system.tpp> // Template function definitions.

#endif // SPARK_BASE_COMPONENT_SYSTEM_H

