
#ifndef ENTITY_MANAGER_TPP
#define ENTITY_MANAGER_TPP

namespace ECS::Entities {
    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManager::RegisterCallback(CallbackType callbackType, Class *classInstance, ReturnType(Class::* memberFunction)(FunctionArguments...)) {
        _data->RegisterCallback<Class, ReturnType, FunctionArguments...>(callbackType, classInstance, memberFunction);
    }

    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::AddComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function AddComponent: Invalid component type provided.");
        _data->AddComponent<ComponentType>(ID);
    }

    // Attach a component to an entity at the provided name, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::AddComponent(std::string name) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function AddComponent: Invalid component type provided.");
        _data->AddComponent<ComponentType>(std::move(name));
    }

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::DeleteComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function DeleteComponent: Invalid component type provided.");
        _data->DeleteComponent<ComponentType>(ID);
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::DeleteComponent(std::string name) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function DeleteComponent: Invalid component type provided.");
        _data->DeleteComponent<ComponentType>(std::move(name));
    }
}

#endif // ENTITY_MANAGER_TPP
