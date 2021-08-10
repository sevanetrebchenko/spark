
#ifndef SPARK_COMPONENT_MANAGER_TPP
#define SPARK_COMPONENT_MANAGER_TPP

#include "spark/core/utility.h"
#include "spark/ecs/components/types/base_component.h"

namespace Spark::ECS {

    template <class ComponentType>
    ComponentManager<ComponentType>::ComponentManager() {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<IComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from IComponent.");
    }

    template <class ComponentType>
    ComponentManager<ComponentType>::~ComponentManager() {
        // Erase all elements.
        typename std::forward_list<ComponentType>::iterator previous = components_.before_begin();
        for (typename std::forward_list<ComponentType>::iterator current = components_.begin(); current != components_.end(); ++current) {
            current = components_.erase_after(previous);
            previous = current;
            ++current;
        }
    }

    template <class ComponentType>
    ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        ComponentType* component = new ComponentType(); // Requires default constructor.
        components_.push_front(component);
        return component;
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
