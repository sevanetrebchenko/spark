
#ifndef COMPONENT_MANAGER_COLLECTION_TPP
#define COMPONENT_MANAGER_COLLECTION_TPP

#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

#include "../../World/all_components.h"

namespace ECS::Components {
    template <class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::_componentManagerCollection = nullptr;

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::GetInstance() {
        if (!_componentManagerCollection) {
            _componentManagerCollection = new ComponentManagerCollection<ALL_COMPONENTS>();
            _componentManagerCollection->Initialize();
        }

        return _componentManagerCollection;
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() {
        static_assert((std::is_base_of_v<Components::BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ECS::Components::BaseComponent.");

    }

    template<class... ComponentTypes>
    template<class ComponentType>
    inline void ComponentManagerCollection<ComponentTypes...>::CreateComponentSystem(unsigned& index) {
        // Construct component manager of given type in-place.
        ComponentManager<ComponentType>* componentManagerBlock = _componentManagerStorage.operator[]<ComponentManager<ComponentType>>(index++);
        new (componentManagerBlock) ComponentManager<ComponentType>();
        componentManagerBlock->Initialize();

        // Set index in collection map.
        _componentManagerMap[ComponentType::ID] = reinterpret_cast<ComponentManagerInterface*>(componentManagerBlock);
    }

    template<class... ComponentTypes>
    inline void ComponentManagerCollection<ComponentTypes...>::Initialize() {
        // Initialize array storage.
        _componentManagerStorage.Initialize();

        // Initialize systems.
        unsigned index = 0;
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes, index);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    inline ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::GetComponentManager() {
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to GetComponentManager - there is no component manager for the given component type.");

        auto it = _componentManagerMap.find(ComponentType::ID);
        if (it != _componentManagerMap.end()) {
            return dynamic_cast<ComponentManager<ComponentType>*>(it->second);
        }
        else {
            return nullptr;
        }
    }
}

#endif // COMPONENT_MANAGER_COLLECTION_TPP
