
#ifndef COMPONENT_MANAGER_TPP
#define COMPONENT_MANAGER_TPP

#include "../../../UtilityBox/Memory/include/segmented_pool_allocator.h" // SegmentedPoolAllocator
#include "../../../UtilityBox/Logger/include/logging_system.h"           // LoggingSystem
#include "../../../UtilityBox/Logger/include/log_message.h"              // LogMessage, LogMessageSeverity
#include "../../../UtilityBox/Tools/global_defines.h"                    // _NODISCARD_
#include "../../Components/include/base_component.h"                    // BaseComponent

namespace ECS::Components {
    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ComponentType>
    class ComponentManager<ComponentType>::ComponentManagerData {
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
            UtilityBox::Memory::SegmentedPoolAllocator _allocator { sizeof(ComponentType) }; // Memory manager.
            UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("ComponentManager: ") + std::string(ComponentType::Name) }; // Logging system.
    };

    // Creates logging system memory allocator for components managed by this system.
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::ComponentManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Constructing ComponentManager managing component type: '%s'.", ComponentType::Name);
        _loggingSystem.Log(message);
    }

    // Cleans up allocator and logging system.
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::~ComponentManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Destroying ComponentManager managing component type: '%s'.", ComponentType::Name);
        _loggingSystem.Log(message);
    }

    // Uses the memory manager to retrieve a block of memory and default constructs a component in-place.
    template<class ComponentType>
    inline ComponentType *ComponentManager<ComponentType>::ComponentManagerData::CreateComponent() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function CreateComponent of type: '%s'.", ComponentType::Name);

        // Program can run out of memory if underlying allocator has no more free blocks in it's pages and RetrieveBlock
        // fails to reallocate a new block page.
        void* blockAddress;
        try {
            blockAddress = _allocator.RetrieveBlock();
        }
        catch (std::bad_alloc& e) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Allocator failed to reallocate new block page - program is out of memory.");
            _loggingSystem.Log(message);

            throw e;
        }

        // Default construct the block in-place.
        new (blockAddress) ComponentType();
        message.Supply("Free block retrieved, object successfully default-constructed in-place.");
        _loggingSystem.Log(message);

        return static_cast<ComponentType*>(blockAddress);
    }

    // Returns a component pointed at by the provided pointer back to the memory manager. Calls destructor for the object.
    template<class ComponentType>
    inline void ComponentManager<ComponentType>::ComponentManagerData::DeleteComponent(ComponentType *component) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function DeleteComponent.");

        if (component) {
            message.Supply("Valid component provided to DeleteComponent. Calling destructor on component.");
            component->~ComponentType();

            message.Supply("Returning component block back to Allocator.");
            _allocator.ReturnBlock(static_cast<void*>(component));
        }
        else {
            // Issue warning message.
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
            message.Supply("Pointer provided to DeleteComponent is null.");
        }

        _loggingSystem.Log(message);
    }


    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER
    //------------------------------------------------------------------------------------------------------------------
    // Ensures desired component type for the ComponentManager is valid before any initialization happens. Creates the
    // back-end functionality and allocates space for components.
    template<class ComponentType>
    inline ComponentManager<ComponentType>::ComponentManager() : _data(new ComponentManagerData()) /* Throws exception, not caught purposefully. */ {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ECS::Components::BaseComponent.");
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

#endif // COMPONENT_MANAGER_TPP
