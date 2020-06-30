
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
#define DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include <unordered_map>
#include <typeindex>
#include "../../Utilities/Memory/segmented_pool_allocator.h"
#include "component_manager.h"
#include "../ecs_typedefs.h"
#include "base_component.h"
#include "../../Utilities/DataStructures/Array/array.h"

#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

namespace ECS::Components {
    template <class ...ComponentTypes>
    class ComponentManagerCollection {
        public:
            ComponentManagerCollection() = default;
            void Initialize();

            template <class ComponentType>
            ComponentManager<ComponentType>* GetComponentManager();

        private:
            template <class ComponentType>
            void CreateComponentSystem(unsigned& index);

            UtilityBox::DataStructures::Array _componentManagerStorage { sizeof(ComponentManager<BaseComponent>), sizeof...(ComponentTypes) };
            std::unordered_map<ECS::ComponentTypeID, ComponentManagerInterface*> _componentManagers;
    };
}

#endif //DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include "component_manager_collection.tpp"
