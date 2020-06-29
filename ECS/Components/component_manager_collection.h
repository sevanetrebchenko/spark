
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
#define DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include <unordered_map>
#include <typeindex>
#include "../../Utilities/Memory/segmented_pool_allocator.h"
#include "component_manager.h"
#include "../ecs_typedefs.h"
#include "base_component.h"

#define FOR_EACH(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

class Component : public ECS::Components::BaseComponent {

};

namespace ECS::Components {
    template <class ...ComponentType>
    class ComponentManagerCollection {
        public:
            ComponentManagerCollection();
        private:

            std::unordered_map<ECS::ComponentTypeID, ComponentManagerInterface*> _componentManagers;
    };
}



#endif //DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
