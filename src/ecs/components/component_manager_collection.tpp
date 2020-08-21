
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_TPP
#define SPARK_COMPONENT_MANAGER_COLLECTION_TPP

#include <spark_pch.h>                                  // std::unordered_map
#include <ecs/all_components.h>                         // ALL_COMPONENTS
#include <ecs/components/component_manager_interface.h> // ComponentManagerInterface
#include <ecs/ecs_typedefs.h>                           // ComponentTypeID
#include <ecs/components/base_component.h>              // BaseComponent
#include <utilitybox/tools/utility_functions.h>         // PARAMETER_PACK_EXPAND
#include <utilitybox/logger/logging_interface.h>        // ILoggable

namespace Spark::ECS::Components {
    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER COLLECTION DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ...ComponentTypes>
    class ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData : public UtilityBox::Logger::ILoggable {
        public:
            /**
             * Registers a ComponentManager for each unique component type in the system's template argument list.
             */
            ComponentManagerCollectionData();

            /**
             * Cleans up all resources and unregisters all registered ComponentManager instances.
             */
            ~ComponentManagerCollectionData();

            /**
             * Get a ComponentManager for a specific component type.
             * @tparam ComponentType - Desired type of the ComponentManager.
             * @return Pointer to component manager.
             */
            template <class ComponentType>
            ComponentManager<ComponentType>* GetComponentManager();

        private:
            /**
             * Constructs and registers a unique ComponentManager for the provided component type.
             * @tparam ComponentType - Component type for ComponentManager to manage.
             */
            template <class ComponentType>
            void CreateComponentSystem();

            /**
             * Safely cleans up a ComponentManager with the provided component type.
             * @tparam ComponentType - Component type for Component Manager to manage.
             */
            template <class ComponentType>
            void DestroyComponentSystem();

            std::unordered_map<ComponentTypeID, ComponentManagerInterface*> _componentManagerMap; // Map for getting individual ComponentManagers.
    };

    // Static instance initialization.
    template <class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::_componentManagerCollection = nullptr;

    // Registers a ComponentManager for each unique component type in the system's template argument list.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::ComponentManagerCollectionData() : UtilityBox::Logger::ILoggable("Component Manager Collection") {
        // Initialize systems.
        LogDebug("Constructing all ComponentManagers.");
        PARAMETER_PACK_EXPAND(CreateComponentSystem, ComponentTypes);
        LogDebug("Finished constructing ComponentManagers.");
    }

    // Cleans up all resources and unregisters all registered ComponentManager instances.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::~ComponentManagerCollectionData() {
        _componentManagerMap.clear();

        // Destroy systems.
        LogDebug("Destroying all ComponentManagers.");
        PARAMETER_PACK_EXPAND(DestroyComponentSystem, ComponentTypes);
        LogDebug("Finished destroying all ComponentManagers.");
    }

    // Get a ComponentManager for a specific component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    ComponentManager<ComponentType>* ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::GetComponentManager() {
        // Component system will always be found due to static assert in ComponentManagerCollection ensuring the component manager collection
        // manages this type of component.
        return dynamic_cast<ComponentManager<ComponentType>*>(_componentManagerMap.find(ComponentType::ID)->second);
    }

    // Constructs and registers a unique ComponentManager for the provided component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    inline void ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::CreateComponentSystem() {
        // Construct component manager of given type.
        LogDebug("Constructing ComponentManager of component type: '%s'.", ComponentType::Name);

        try {
            ComponentManager<ComponentType>* componentManager = new ComponentManager<ComponentType>();

            // Register in component manager map.
            _componentManagerMap[ComponentType::ID] = componentManager;
        }
        catch (std::bad_alloc& e) {
            // Failure to allocate component manager is a fatal error.
            LogError("Exception thrown: Failed to allocate sufficient memory for back-end of ComponentManager with component type: '%s'.", ComponentType::Name);
            throw e;
        }
    }

    // Safely cleans up a ComponentManager with the provided component type.
    template<class... ComponentTypes>
    template<class ComponentType>
    void ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollectionData::DestroyComponentSystem() {
        LogDebug("Destroying ComponentManager of component type: '%s'.", ComponentType::Name);
        _componentManagerMap.erase(ComponentType::ID);
    }


    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER COLLECTION
    //------------------------------------------------------------------------------------------------------------------
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>* ComponentManagerCollection<ComponentTypes...>::GetInstance() {
        Initialize();
        return _componentManagerCollection;
    }

    // Direct call to initialize resources necessary for the ComponentManagerCollection to function properly.
    template<class... ComponentTypes>
    void ComponentManagerCollection<ComponentTypes...>::Initialize() {
        if (!_componentManagerCollection) {
            _componentManagerCollection = new ComponentManagerCollection<ALL_COMPONENTS>(); // Throws on error, caught elsewhere.
        }
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
    ComponentManagerCollection<ComponentTypes...>::ComponentManagerCollection() : _data(new ComponentManagerCollectionData()) {
        static_assert((std::is_base_of_v<Components::BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ecs::components::BaseComponent.");
    }

    // Private destructor for singleton class instance.
    template<class... ComponentTypes>
    ComponentManagerCollection<ComponentTypes...>::~ComponentManagerCollection() {
        delete _data;
    }
}

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_TPP
