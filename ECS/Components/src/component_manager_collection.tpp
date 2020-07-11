
#ifndef COMPONENT_MANAGER_COLLECTION_TPP
#define COMPONENT_MANAGER_COLLECTION_TPP

#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

#include "../../../World/all_components.h"                  // ALL_COMPONENTS
#include "../include/component_manager_interface.h"         // ComponentManagerInterface
#include "../../ecs_typedefs.h"                             // ComponentTypeID
#include "../../Components/include/base_component.h"        // BaseComponent
#include "../../../Utilities/Logger/logging_system.h"       // LoggingSystem
#include <unordered_map>                                    // std::unordered_map
#include "../../../Utilities/Memory/contiguous_pool_allocator.h"

namespace ECS::Components {
    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER COLLECTION DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ...ComponentTypes>
    class ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData {
        public:
            /**
             * Constructor (default).
             */
            ComponentManagerCollectionData() = default;

            /**
             * Cleans up all resources and Component Managers managed by the Component Manager Collection.
             */
            ~ComponentManagerCollectionData();

            /**
             * Two-stage initialization. Creates and initializes all individual Component Managers.
             */
            void Initialize();

            /**
             * Get a Component Manager for a specific component type.
             * @tparam ComponentType - Desired type of the Component Manager.
             * @return Pointer to component manager.
             */
            template <class ComponentType>
            ComponentManager<ComponentType>* GetComponentManager();

            UtilityBox::Logger::LoggingSystem _loggingSystem { "Component Manager Collection" }; // ComponentManagerCollection logging system.

        private:
            /**
             * Retrieve a block from the memory manager and construct an individual Component Manager for a given component type.
             * @tparam ComponentType - Component type for Component Manager to manage.
             * @param  index         - Index of the _componentManagerStorage array to construct the ComponentManager in.
             */
            template <class ComponentType>
            void CreateComponentSystem(unsigned& index, UtilityBox::Logger::LogMessage& message);

            /**
             * Destroy an individual ComponentManager and return the block back to the memory manager.
             * @tparam ComponentType - Component type for Component Manager to manage.
             * @param  index         - Index of the _componentManagerStorage array to d the ComponentManager in.
             */
            template <class ComponentType>
            void DestroyComponentSystem(unsigned& index, UtilityBox::Logger::LogMessage& message);

            UtilityBox::Memory::ContiguousPoolAllocator _componentManagerStorage;                      // Storage for component managers.
            std::unordered_map<ComponentTypeID, ComponentManagerInterface*> _componentManagerMap;      // Map for getting individual ComponentManagers.
    };

    template <class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::_componentManagerCollection = nullptr;

    // Cleans up all resources and Component Managers managed by the Component Manager Collection.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::~ComponentManagerCollectionData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering ComponentManagerCollection destructor.");
        message.Supply("Clearing component manager pointer map.");
        _componentManagerMap.clear();

        // Destroy systems.
        unsigned index = 0;
        PARAMETER_PACK_EXPAND(DestroyComponentSystem, ComponentTypes, index, message);

        message.Supply("Finished destroying all ComponentManagers.");
        _loggingSystem.Log(message);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    inline void ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::CreateComponentSystem(unsigned& index, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Entering function CreateComponentSystem with component type: '%s'.", ComponentType::Name);

        // Construct component manager of given type in-place.
        void* componentManagerBlock = _componentManagerStorage[index];
        message.Supply("Creating ComponentManager in place in index: %s of contiguous memory storage array.", index);
        new (componentManagerBlock) ComponentManager<ComponentType>();

        message.Supply("Initializing component system.");
        static_cast<ComponentManager<ComponentType>*>(componentManagerBlock)->Initialize();

        // Set index in collection map.
        message.Supply("Setting component index in collection's component manager map.");
        _componentManagerMap[ComponentType::ID] = reinterpret_cast<ComponentManagerInterface*>(componentManagerBlock);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::DestroyComponentSystem(unsigned& index, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Entering function DestroyComponentSystem with component type: '%s'.", ComponentType::Name);

        message.Supply("Retrieving ComponentManager at index: %i.", index);
        ComponentManager<ComponentType>* componentManagerBlock = static_cast<ComponentManager<ComponentType>*>(_componentManagerStorage[index++]);

        // Destroy component before returning block back to memory manager.
        message.Supply("Calling destructor for ComponentManager.");
        componentManagerBlock->~ComponentManager<ComponentType>();

        message.Supply("Returning block to memory manager.");
        _componentManagerStorage.ReturnBlock(componentManagerBlock);
        componentManagerBlock = nullptr;
    }

    template<class... ComponentTypes>
    void ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::Initialize() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function Initialize.");

        // Initialize contiguous storage.
        unsigned blockSize = sizeof(ComponentManager<BaseComponent>);
        unsigned numBlocks = sizeof...(ComponentTypes);
        message.Supply("Initializing continuous memory allocator with %i blocks, each with size %i bytes.");
        _componentManagerStorage.Initialize(blockSize, numBlocks, false);

        // Initialize systems.
        unsigned index = 0;
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes, index, message);

        message.Supply("Finished constructing all ComponentManagers.");
        _loggingSystem.Log(message);
    }

    template<class... ComponentTypes>
    template<class ComponentType>
    ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::GetComponentManager() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetComponentManager with component type: '%s'.", ComponentType::Name);
        _loggingSystem.Log(message);

        // Component system will always be found due to static assert in ComponentManagerCollection ensuring the component manager collection
        // manages this type of component.
        return dynamic_cast<ComponentManager<ComponentType>*>(_componentManagerMap.find(ComponentType::ID)->second);
    }


    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER COLLECTION
    //------------------------------------------------------------------------------------------------------------------
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::GetInstance() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetInstance.");
        if (!_componentManagerCollection) {
            message.Supply("ComponentManagerCollection not constructed. Constructing...");
            _componentManagerCollection = new ComponentManagerCollection<ALL_COMPONENTS>();

            message.Supply("ComponentManagerCollection not initialized. Initializing...");
            try {
                _componentManagerCollection->Initialize();
            }
            catch (std::bad_alloc& exceptionAllocation) {
                // Record instance.
                message.Supply("ComponentManagerCollection failed to allocate memory for back-end functionality and data. Provided exception message: '%s'.", exceptionAllocation.what());
                message.Supply("Error message issued.");
                _componentManagerCollection->_data->_loggingSystem.Log(message);

                UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
                errorMessage.Supply("Exception thrown: function GetInstance in ComponentManagerCollection failed to allocate memory.");
                _componentManagerCollection->_data->_loggingSystem.Log(errorMessage);

                // Re-throw exception.
                throw exceptionAllocation;
            }
        }
        else {
            message.Supply("ComponentManagerCollection was already initialized. Returning instance.");
        }

        _componentManagerCollection->_data->_loggingSystem.Log(message);
        return _componentManagerCollection;
    }

    // Get a Component Manager for a specific component type. Function ensures the desired component type is present and
    // managed by the Component Manager Collection.
    template<class... ComponentTypes>
    template<class ComponentType>
    inline ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::GetComponentManager() {
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to GetComponentManager - there is no component manager for the given component type.");
        return _data->template GetComponentManager<ComponentType>();
    }

    // Private constructor for singleton class instance.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() {
        static_assert((std::is_base_of_v<Components::BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ECS::Components::BaseComponent.");

        // Defer initialization to second stage.
    }

    // Private destructor for singleton class instance.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::~ComponentManagerCollection() {
        delete _data;
    }

    // Creates and initializes back-end functionality for the Component Manager Collection. Private to enforce singleton
    // class instance.
    template<class... ComponentTypes>
    inline void ComponentManagerCollection<ComponentTypes...>::Initialize() {
        if (!_data) {
            _data = new ComponentManagerCollectionData();
            // TODO: try catch
            _data->Initialize();
        }
    }
}

#endif // COMPONENT_MANAGER_COLLECTION_TPP
