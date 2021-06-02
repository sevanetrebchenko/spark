
#ifndef SPARK_COMPONENT_MANAGER_TPP
#define SPARK_COMPONENT_MANAGER_TPP

#include <spark/core/core.h>
#include <spark/utilitybox/logger/logging_interface.h> // ILoggable
#include <spark/ecs/components/types/base_component.h> // BaseComponent

namespace Spark::ECS::Components {

    // Ensures desired component type for the ComponentManager is valid before any initialization happens. Creates the
    // back-end functionality and allocates space for components.
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManager() : UtilityBox::Logger::ILoggable(std::string("ComponentManager: ") + std::string(ComponentType::Name)) {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ecs::components::BaseComponent.");
        LogDebug("Constructing ComponentManager of component type: '%s'.", ComponentType::Name);
    }

    // Safely cleans up back-end functionality and allocated component memory space.
    template<class ComponentType>
    ComponentManager<ComponentType>::~ComponentManager() {
        LogDebug("Destroying ComponentManager of component type: '%s'.", ComponentType::Name);
    }

    // Retrieves and default-constructs a component to return to the user.
    template<class ComponentType>
    ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        // Program can run out of memory if underlying allocator has no more free blocks in it's pages and RetrieveBlock fails to reallocate a new block page.
        void* blockAddress;
        try {
            blockAddress = _allocator.RetrieveBlock();
        }
        catch (std::bad_alloc& e) {
            LogError("Exception thrown: Allocator failed to reallocate new block page - program is out of memory.");
            throw e;
        }

        // Default construct the block in-place.
        new (blockAddress) ComponentType();
        return static_cast<ComponentType*>(blockAddress);
    }

    // Return the memory associated with a component back to the ComponentManager to be reused. Automatically calls
    // component destructor before returning memory back.
    template<class ComponentType>
    void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        if (component) {
            component->~ComponentType();
            _allocator.ReturnBlock(static_cast<void*>(component));
        }
    }

}

#endif // SPARK_COMPONENT_MANAGER_TPP
