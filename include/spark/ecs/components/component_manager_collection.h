
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_H
#define SPARK_COMPONENT_MANAGER_COLLECTION_H

#include "component_manager.h" // ComponentManager

namespace Spark {
    namespace ECS {
        namespace Components {

            template <class ...ComponentTypes>
            class ComponentManagerCollection {
                public:
                    /**
                    * Returns singleton instance of the ComponentManagerCollection. Instantiates singleton if necessary.
                    * @return Pointer to singleton instance of the ComponentManagerCollection.
                    */
                    static ComponentManagerCollection* GetInstance();

                    /**
                    * Direct call to initialize resources necessary for the ComponentManagerCollection to function properly.
                    */
                    static void Initialize();

                    /**
                    * Get a ComponentManager for a specific component type. Function ensures the desired component type is
                    * present and managed by the ComponentManagerCollection.
                    * @tparam ComponentType - Desired type of the Component Manager.
                    * @return Pointer to component manager.
                    */
                    template <class ComponentType>
                    ComponentManager<ComponentType>* GetComponentManager();

                private:
                    /**
                    * Private constructor for singleton class instance.
                    */
                    ComponentManagerCollection();

                    /**
                    * Private destructor for singleton class instance.
                    */
                    ~ComponentManagerCollection();

                    static ComponentManagerCollection* _componentManagerCollection; // Singleton.

                    // Storage for ComponentManager data, back-end functionality, and helper functions.
                    class ComponentManagerCollectionData;
                    ComponentManagerCollectionData* _data = nullptr;
            };

        } // namespace Components
    } // namespace ECS
} // namespace Spark

#include "../../../../src/ecs/components/component_manager_collection.tpp" // Template function includes.

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_H

