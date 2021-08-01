
#ifndef SPARK_COMPONENT_MANAGER_TPP
#define SPARK_COMPONENT_MANAGER_TPP

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/logging_interface.h>
#include <spark/ecs/components/types/base_component.h>

namespace Spark::ECS {

    template <class ComponentType>
    ComponentManager<ComponentType>::ComponentManager() : UtilityBox::Logger::ILoggable(std::string("ComponentManager: ") + std::string(ComponentType::Name)) {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from BaseComponent.");
    }

    template <class ComponentType>
    ComponentManager<ComponentType>::~ComponentManager() {
    }

    template <class ComponentType>
    ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        components_.emplace_front(new ComponentType()); // Requires default constructor.
    }

    template <class ComponentType>
    void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        if (!component) {
            return;
        }

        typename std::forward_list<ComponentType>::iterator previous = components_.before_begin();
        for (typename std::forward_list<ComponentType>::iterator current = components_.begin(); current != components_.end(); ++current) {
            if (&(*current) == component) {
                components_.erase_after(previous);
                break;
            }

            previous = current;
        }
    }

}

#endif // SPARK_COMPONENT_MANAGER_TPP
