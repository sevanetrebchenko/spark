
#include <type_traits>
#include "component_manager.h"

namespace ECS {
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManager() : _allocator(nullptr) {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid type provided to ComponentManager.");

        // Defer initialization to second stage.
    }

    template <typename ComponentType>
    void ComponentManager<ComponentType>::Initialize() {
        _allocator = new UtilityBox::Memory::SegmentedPoolAllocator(sizeof(ComponentType));
        _allocator->Initialize();
    }

    template<class ComponentType>
    ComponentManager<ComponentType>::~ComponentManager() {
        delete _allocator;
    }

    template<class ComponentType>
    ComponentType* ComponentManager<ComponentType>::AddComponent() {
        return static_cast<ComponentType*>(_allocator->RetrieveBlock());
    }

    template<class ComponentType>
    void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        _allocator->ReturnBlock(component);
    }
}
