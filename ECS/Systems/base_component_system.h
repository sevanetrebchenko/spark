
#ifndef DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H
#define DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include <tuple>
#include <vector>
#include "../../Utilities/Memory/segmented_pool_allocator.h"

namespace ECS::Components {
    template <class ...ComponentTypes>
    class ComponentSystem {
        public:
            ComponentSystem();
            ~ComponentSystem();

            virtual void Initialize();
            virtual void Update(float dt) = 0;

            void OnEntityCreate(EntityID ID);
            void OnEntityDestroy(EntityID ID);
            void OnEntityComponentAdd(EntityID ID);
            void OnEntityComponentRemove(EntityID ID);

        protected:
            using ComponentTuple = std::tuple<std::add_pointer<ComponentTypes>...>; // Tuple of component pointers.
            std::vector<ComponentTuple> _filteredEntities;

        private:
            void FilterEntity(ECS::EntityID ID);

            template <unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
            bool ProcessEntityComponent(ComponentTypeID, BaseComponent* component, ComponentTuple& componentTuple);

            template <unsigned INDEX>
            bool ProcessEntityComponent(ComponentTypeID, BaseComponent* component, ComponentTuple& componentTuple);

            std::unordered_map<EntityID, unsigned> _entityIDToContainerIndex; // Mapping from entity ID to index in filtered entities vector.
            std::unordered_map<unsigned, EntityID> _containerIndexToEntityID; // Mapping from index in filtered entities vector to entity ID.

    };
}

#endif //DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include "base_component_system.tpp"
