
#ifndef SPARK_BASE_COMPONENT_SYSTEM_H
#define SPARK_BASE_COMPONENT_SYSTEM_H

#include <spark_pch.h>                                   // std::string, std::tuple, std::vector
#include <spark/utilitybox/logger/logging_interface.h>   // ILoggable
#include <ecs/systems/base_component_system_interface.h> // BaseComponentSystemInterface

namespace Spark {
    namespace ECS {
        namespace Systems {

            template <class ...ComponentTypes>
            class BaseComponentSystem : public BaseComponentSystemInterface, UtilityBox::Logger::ILoggable {
                public:
                    /**
                    * Constructs the necessary fundamentals for a component system, including automatic entity filtering and
                    * logging system instance. Enforces all component types managed by this system are derived from BaseComponent.
                    * @param systemName - Name for this component system.
                    */
                    explicit BaseComponentSystem(std::string systemName);

                    /**
                    * Destructor.
                    */
                    ~BaseComponentSystem();

                    /**
                    * Initialize the logging system and any supplemental back-end functionality.
                    */
                    void Initialize() override;

                    /**
                    * Update.
                    * @param dt - Time-step since last frame.
                    */
                    void Update(float dt) override;

                    /**
                    * Shutdown any resources associated with this component system.
                    */
                    void Shutdown() override;

                protected:
                    using BaseSystem = BaseComponentSystem<ComponentTypes...>;
                    using ComponentTuple = std::tuple<ComponentTypes*...>; // Tuple of component pointers.

                    /**
                    * Get a specific component pointer from the tuple at the provided index. Function ensures the desired
                    * component is managed by the system.
                    * @tparam ComponentType - Desired component type to retrieve.
                    * @param index          - Index into the component tuple array to get the component from.
                    * @return Pointer to the desired component type.
                    */
                    template <typename ComponentType>
                    ComponentType* GetComponent(unsigned index);

                    /**
                    * Get a specific component pointer from the tuple at the provided index. Function ensures the desired
                    * component is managed by the system.
                    * @tparam ComponentType  - Desired component type to retrieve.
                    * @param  componentTuple - Component tuple to get the component from.
                    * @return Pointer to the desired component type.
                    */
                    template <typename ComponentType>
                    ComponentType* GetComponent(const ComponentTuple& componentTuple);

                    std::vector<ComponentTuple> _filteredEntities; // Array of component tuples of entities managed by this system.

                private:
                    // Storage for BaseComponentSystem data, back-end functionality, and helper functions.
                    class BaseComponentSystemData;
                    BaseComponentSystemData* _data = nullptr;
            };

        }
    }
}

#include <ecs/systems/base_component_system.tpp> // Template function definitions.

#endif // SPARK_BASE_COMPONENT_SYSTEM_H

