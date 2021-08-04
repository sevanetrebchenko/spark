
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_TPP
#define SPARK_COMPONENT_MANAGER_COLLECTION_TPP

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/logging_interface.h>         // ILoggable
#include <spark/ecs/components/component_manager_interface.h>  // IComponentManager

namespace Spark::ECS {

    template<class... ComponentTypes>
    template<class ComponentType>
    ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::GetComponentManager() {
        static_assert((std::is_base_of_v<BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to GetComponentManager - component types must derive from BaseComponent.");
        static_assert((std::is_same_v<BaseComponent, ComponentTypes> || ...), "Invalid template parameter provided to GetComponentManager - component type is not currently registered in ComponentManagerCollection.");
        return dynamic_cast<ComponentManager<ComponentType>*>(componentManagerMap_.find(ComponentType::ID)->second);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::CreateComponentSystem() {
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
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() : UtilityBox::Logger::ILoggable("Component Manager Collection") {
        static_assert((std::is_base_of_v<BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from BaseComponent.");
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes);
    }

    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::~ComponentManagerCollection() {
        PARAMETER_PACK_EXPAND(DestroyComponentSystem, ComponentTypes);
    }

}

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_TPP
