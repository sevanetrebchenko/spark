
#ifndef COMPONENT_MANAGER_COLLECTION_TPP
#define COMPONENT_MANAGER_COLLECTION_TPP

#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

namespace ECS::Components {
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
        std::unordered_map<ComponentTypeID, ComponentManagerInterface*>::iterator it = _componentManagerMap.find(ComponentType::ID);
        if (it != _componentManagerMap.end()) {
            return dynamic_cast<ComponentManager<ComponentType>*>(it->second);
        }
        else {
            return nullptr;
        }
    }
}

#endif // COMPONENT_MANAGER_COLLECTION_TPP
