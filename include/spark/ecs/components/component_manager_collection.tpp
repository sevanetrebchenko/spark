
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_TPP
#define SPARK_COMPONENT_MANAGER_COLLECTION_TPP

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/logging_interface.h>         // ILoggable
#include <spark/ecs/components/component_manager_collection.h> // ComponentManagerCollection
#include <spark/ecs/components/component_manager_interface.h>  // IComponentManager

namespace Spark::ECS {

    template<class... ComponentTypes>
    template<class ComponentType>
    ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::GetComponentManager() {
        // Component system will always be found due to static assert in ComponentManagerCollection.
        return dynamic_cast<ComponentManager<ComponentType>*>(componentManagerMap_.find(ComponentType::ID)->second);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::CreateComponentSystem() {
        // Construct component manager of given type.
        LogDebug("Constructing ComponentManager of component type: '%s'.", ComponentType::Name);
        componentManagerMap_[ComponentType::ID] = new ComponentManager<ComponentType>();
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::DestroyComponentSystem() {
        LogDebug("Destroying ComponentManager of component type: '%s'.", ComponentType::Name);
        delete componentManagerMap_[ComponentType::ID];
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::GetInstance() {
        static ComponentManagerCollection<ComponentTypes...> instance;
        return &instance;
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() : UtilityBox::Logger::ILoggable("Component Manager Collection") {
        static_assert((std::is_base_of_v<BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ecs::components::BaseComponent.");
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes);
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::~ComponentManagerCollection() {
        PARAMETER_PACK_EXPAND(DestroyComponentSystem, ComponentTypes);
    }

}

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_TPP
