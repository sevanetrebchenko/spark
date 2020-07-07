
#ifndef ENTITY_MANAGER_TPP
#define ENTITY_MANAGER_TPP

namespace ECS::Entities {

    template<class ComponentType>
    inline void EntityManager::AddComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

//        _entities.at(ID).insert(ComponentType::ID, ENGINE_NAME::World::GetInstance().GetEn

        // Call all callback functions to notify systems.
        for (auto &componentAddCallbackFunction : _componentAddCallbackFunctions) {
            componentAddCallbackFunction(ID);
        }
    }

    template<class ComponentType>
    inline void EntityManager::DeleteComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

        // Component found - delete.
//            _entities.at(ID).insert(ComponentType::ID, // get component here);

        // Call all callback functions to notify systems.
        for (auto &componentRemoveCallbackFunction : _componentRemoveCallbackFunctions) {
            componentRemoveCallbackFunction(ID);
        }
    }

    template<class Class, typename ReturnType, typename... Args>
    auto EntityManager::GetMemberFunction(Class *classInstance, ReturnType(Class::* memberFunction)(Args...)) {
        return [classInstance, memberFunction](Args... additionalArguments) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function correctly.
            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
        };
    }

    template<class Class, typename ReturnType, typename... FunctionArguments>
    void EntityManager::RegisterCallback(EntityManager::CallbackType callbackType, Class *classInstance, ReturnType(Class::* memberFunction)(FunctionArguments...)) {
        // Construct lambda expression capturing class 'this' pointer and member function to call.
        std::function<void(EntityID)> function = GetMemberFunction<Class, ReturnType, EntityID>(classInstance, memberFunction);

        // Append the function to the correct location.
        switch (callbackType) {
            case CallbackType::ENTITY_CREATE:
                _entityCreateCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::ENTITY_DELETE:
                _entityDestroyCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::COMPONENT_ADD:
                _componentAddCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::COMPONENT_REMOVE:
                _componentRemoveCallbackFunctions.emplace_back(std::move(function));
                break;
        }
    }
}

#endif // ENTITY_MANAGER_TPP
