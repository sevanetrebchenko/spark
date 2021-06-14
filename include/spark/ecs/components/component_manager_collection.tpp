
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_TPP
#define SPARK_COMPONENT_MANAGER_COLLECTION_TPP

#include <spark/core/core.h>
#include <spark/utilitybox/logger/logging_interface.h>         // ILoggable
#include <spark/ecs/components/component_manager_collection.h> // ComponentManagerCollection
#include <spark/ecs/components/component_manager_interface.h>  // IComponentManager

namespace Spark::ECS::Components {

    // Get a ComponentManager for a specific component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::GetComponentManager() {
        // Component system will always be found due to static assert in ComponentManagerCollection ensuring the component manager collection
        // manages this type of component.
        return dynamic_cast<ComponentManager<ComponentType>*>(_componentManagerMap.find(ComponentType::ID)->second);
    }

    // Constructs and registers a unique ComponentManager for the provided component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::CreateComponentSystem() {
        // Construct component manager of given type.
        LogDebug("Constructing ComponentManager of component type: '%s'.", ComponentType::Name);
        _componentManagerMap[ComponentType::ID] = new ComponentManager<ComponentType>();
    }

    // Safely cleans up a ComponentManager with the provided component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::DestroyComponentSystem() {
        LogDebug("Destroying ComponentManager of component type: '%s'.", ComponentType::Name);
        delete _componentManagerMap[ComponentType::ID];
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::GetInstance() {
        static ComponentManagerCollection<ComponentTypes...> instance;
        return &instance;
    }

    // Private constructor for singleton class instance.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() : UtilityBox::Logger::ILoggable("Component Manager Collection") {
        static_assert((std::is_base_of_v<Components::BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ecs::components::BaseComponent.");

        LogDebug("Constructing all ComponentManagers.");
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes);
    }

    // Private destructor for singleton class instance.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::~ComponentManagerCollection() {
        // Destroy systems.
        LogDebug("Destroying all ComponentManagers.");
        PARAMETER_PACK_EXPAND(DestroyComponentSystem, ComponentTypes);
    }

}

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_TPP
