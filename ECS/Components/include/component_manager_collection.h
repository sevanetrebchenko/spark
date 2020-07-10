
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
#define DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include <unordered_map>
#include "component_manager.h"
#include "../../ecs_typedefs.h"
#include "base_component.h"
#include "../../../Utilities/DataStructures/Array/array.h"
#include "../../../Utilities/Logger/logging_system.h"

namespace ECS::Components {
    template <class ...ComponentTypes>
    class ComponentManagerCollection {
        public:
            static ComponentManagerCollection* GetInstance();

            template <class ComponentType>
            ComponentManager<ComponentType>* GetComponentManager();

        private:
            ComponentManagerCollection();
            ~ComponentManagerCollection();
            void Initialize();

            static ComponentManagerCollection* _componentManagerCollection;

            class ComponentManagerCollectionData;
            ComponentManagerCollectionData* _data;
    };
}

#include "../src/component_manager_collection.tpp" // Template function includes.

#endif //DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

