
#ifndef SPARK_COMPONENT_MANAGER_TPP
#define SPARK_COMPONENT_MANAGER_TPP

#include <spark/core/core.h>
#include <spark/utilitybox/logger/logging_interface.h>        // ILoggable
#include <spark/ecs/components/types/base_component.h>        // BaseComponent
#include <spark/utilitybox/memory/segmented_pool_allocator.h> // SegmentedPoolAllocator

namespace Spark::ECS::Components {
    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ComponentType>
    class ComponentManager<ComponentType>::ComponentManagerData : public UtilityBox::Logger::ILoggable {
        public:
            /**
             * Constructor (default). Creates logging system memory allocator for components managed by this system.
             */
            ComponentManagerData();

            /**
             * Destructor (default). Cleans up allocator and logging system.
             */
            ~ComponentManagerData();

            /**
             * Uses the memory manager to retrieve a block of memory and default constructs a component in-place.
             * @return Pointer to the constructed component.
             */
            _NODISCARD_ ComponentType* CreateComponent();

            /**
             * Return the memory associated with a component back to the allocator to be reused. Automatically
             * calls component destructor before returning memory back.
             * @param component - Pointer to component to return.
             */
            void DeleteComponent(ComponentType* component);

        private:
            UtilityBox::Memory::SegmentedPoolAllocator _allocator { sizeof(ComponentType) }; // Memory allocator.
    };

    // Creates logging system memory allocator for components managed by this system.
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::ComponentManagerData() : UtilityBox::Logger::ILoggable(std::string("ComponentManager: ") + std::string(ComponentType::Name)){
        LogDebug("Constructing ComponentManager managing component type: '%s'.", ComponentType::Name);
    }

    // Cleans up allocator and logging system.
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::~ComponentManagerData() {
        LogDebug("Destroying ComponentManager managing component type: '%s'.", ComponentType::Name);
    }

    // Uses the memory manager to retrieve a block of memory and default constructs a component in-place.
    template<class ComponentType>
    inline ComponentType *ComponentManager<ComponentType>::ComponentManagerData::CreateComponent() {
        LogDebug("Entering function CreateComponent of type: '%s'.", ComponentType::Name);

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
        LogDebug("Free block retrieved, object successfully default-constructed in-place.");
        return static_cast<ComponentType*>(blockAddress);
    }

    // Returns a component pointed at by the provided pointer back to the memory manager. Calls destructor for the object.
    template<class ComponentType>
    inline void ComponentManager<ComponentType>::ComponentManagerData::DeleteComponent(ComponentType *component) {
        LogDebug("Entering function RemoveComponent.");

        if (component) {
            LogDebug("Valid component provided. Calling destructor and returning block back to allocator.");
            component->~ComponentType();
            _allocator.ReturnBlock(static_cast<void*>(component));
        }
        else {
            LogWarning("Pointer provided to RemoveComponent is null.");
        }
    }


    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER
    //------------------------------------------------------------------------------------------------------------------
    // Ensures desired component type for the ComponentManager is valid before any initialization happens. Creates the
    // back-end functionality and allocates space for components.
    template<class ComponentType>
    inline ComponentManager<ComponentType>::ComponentManager() : _data(new ComponentManagerData()) {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ecs::components::BaseComponent.");
    }

    // Safely cleans up back-end functionality and allocated component memory space.
    template<class ComponentType>
    inline ComponentManager<ComponentType>::~ComponentManager() {
        delete _data;
    }

    // Retrieves and default-constructs a component to return to the user.
    template<class ComponentType>
    inline ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        return _data->CreateComponent();
    }

    // Return the memory associated with a component back to the ComponentManager to be reused. Automatically calls
    // component destructor before returning memory back.
    template<class ComponentType>
    inline void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        _data->DeleteComponent(component);
    }
}

#endif // SPARK_COMPONENT_MANAGER_TPP
