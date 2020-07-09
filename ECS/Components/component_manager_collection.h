
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
#define DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include <unordered_map>
#include "component_manager.h"
#include "../ecs_typedefs.h"
#include "base_component.h"
#include "../../Utilities/DataStructures/Array/array.h"

namespace ECS::Components {
    template <class ...ComponentTypes>
    class ComponentManagerCollection {
        public:
            static ComponentManagerCollection* GetInstance();

            template <class ComponentType>
            ComponentManager<ComponentType>* GetComponentManager();

        private:
            ComponentManagerCollection();
            void Initialize();
            ~ComponentManagerCollection();

            template <class ComponentType>
            void CreateComponentSystem(unsigned& index);

            static ComponentManagerCollection* _componentManagerCollection;
            UtilityBox::DataStructures::Array _componentManagerStorage { sizeof(ComponentManager<BaseComponent>), sizeof...(ComponentTypes) };
            std::unordered_map<ComponentTypeID, ComponentManagerInterface*> _componentManagerMap;
            UtilityBox::Logger::LoggingSystem _loggingSystem { "Component Manager Collection" };
    };
}

#include "component_manager_collection.tpp" // Template function includes.

#endif //DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

