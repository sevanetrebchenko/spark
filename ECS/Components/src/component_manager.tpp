
#ifndef COMPONENT_MANAGER_TPP
#define COMPONENT_MANAGER_TPP

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
             * Destructor (default).
             */
            ~ComponentManagerData();

            /**
             * Two-stage initialization. Initializes allocator to the size of the component managed by this system.
             */
            void Initialize();

            /**
             * Uses the memory manager to retrieve a block of memory and default constructs a component in-place.
             * @return Pointer to the constructed component.
             */
            _NODISCARD_ ComponentType* CreateComponent();

            /**
             * Returns a component pointed at by the provided pointer back to the memory manager. Calls destructor for
             * the object.
             * @param component - Object to clean up.
             */
            void DeleteComponent(ComponentType* component);

        private:
            UtilityBox::Memory::SegmentedPoolAllocator _allocator; // Memory manager.
            UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("ComponentManager: ") + std::string(ComponentType::Name) }; // Logging system for this component manager.
    };

    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::ComponentManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered constructor for component manager of component type: '%s'.", ComponentType::Name);
        _loggingSystem.Log(message);
    }

    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManagerData::~ComponentManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered destructor for component manager of component type: '%s'.", ComponentType::Name);
        _loggingSystem.Log(message);
    }

    // Two-stage initialization. Initializes allocator to the size of the component managed by this system.
    template<class ComponentType>
    void ComponentManager<ComponentType>::ComponentManagerData::Initialize() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Initialize.");

        std::size_t blockSize = sizeof(ComponentType);
        message.Supply("Initializing segmented memory allocator for components of type: '%s', sized: %i bytes.", ComponentType::Name, blockSize);
        _loggingSystem.Log(message);

        _allocator.Initialize(blockSize);
    }

    // Uses the memory manager to retrieve a block of memory and default constructs a component in-place.
    template<class ComponentType>
    ComponentType *ComponentManager<ComponentType>::ComponentManagerData::CreateComponent() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function CreateComponent.");

        void* blockAddress = _allocator.RetrieveBlock();

        // Default construct the block in-place.
        new (blockAddress) ComponentType();
        message.Supply("Free block retrieved, object default-constructed in-place.");
        _loggingSystem.Log(message);

        return static_cast<ComponentType*>(blockAddress);
    }

    // Returns a component pointed at by the provided pointer back to the memory manager. Calls destructor for the object.
    template<class ComponentType>
    void ComponentManager<ComponentType>::ComponentManagerData::DeleteComponent(ComponentType *component) {
        UtilityBox::Logger::LogMessage message;
        message.Supply("Entering function DeleteComponent.");

        if (component) {
            message.Supply("");

            component->~ComponentType();
            _allocator.ReturnBlock(static_cast<void*>(component));
        }
        else {
            message.Supply("Pointer provided to DeleteComponent is null. Warning message issued.");

            UtilityBox::Logger::LogMessage warningMessage { UtilityBox::Logger::LogMessageSeverity::WARNING };
            warningMessage.Supply("Entered function DeleteComponent with a null component.");
            _loggingSystem.Log(warningMessage);
        }

        _loggingSystem.Log(message);
    }


    //------------------------------------------------------------------------------------------------------------------
    // COMPONENT MANAGER
    //------------------------------------------------------------------------------------------------------------------
    template<class ComponentType>
    inline ComponentManager<ComponentType>::ComponentManager() {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ECS::Components::BaseComponent.");
        // Defer initialization to second stage.
    }

    template <typename ComponentType>
    inline void ComponentManager<ComponentType>::Initialize() {
        _data->Initialize();
    }

    template<class ComponentType>
    inline ComponentManager<ComponentType>::~ComponentManager() {
        delete _data;
    }

    template<class ComponentType>
    inline ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        return _data->CreateComponent();
    }

    template<class ComponentType>
    inline void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        _data->DeleteComponent(component);
    }
}

#endif // COMPONENT_MANAGER_TPP
