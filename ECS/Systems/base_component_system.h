
#ifndef DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H
#define DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include <tuple>
#include <type_traits>
#include <vector>
#include "../../Utilities/Memory/segmented_pool_allocator.h"

namespace ECS::Components {
    template <class ...ComponentType>
    class ComponentSystem {
        public:
            ComponentSystem();
            ~ComponentSystem();

            virtual void Initialize();
            virtual void Update(float dt) = 0;

            void OnEntityComponentAdd();
            void OnEntityComponentRemove();

        protected:
            using ComponentTuple = std::tuple<std::add_pointer<ComponentType>...>; // Tuple of component pointers.
            std::vector<ComponentTuple*> _filteredEntities;

        private:

    };
}

#endif //DATASTRUCTURES_BASE_COMPONENT_SYSTEM_H

#include "base_component_system.tpp"
