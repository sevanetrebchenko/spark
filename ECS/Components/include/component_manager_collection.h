
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H
#define DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

#include "component_manager.h" // ComponentManager

namespace ECS::Components {
    template <class ...ComponentTypes>
    class ComponentManagerCollection {
        public:
            /**
             * Returns singleton instance of the ComponentManagerCollection. Instantiates singleton if necessary.
             * @return Pointer to singleton instance of the ComponentManagerCollection.
             */
            static ComponentManagerCollection* GetInstance();

            /**
             * Get a Component Manager for a specific component type. Function ensures the desired component type is
             * present and managed by the Component Manager Collection.
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

            /**
             * Creates and initializes back-end functionality for the Component Manager Collection. Private to enforce
             * singleton class instance.
             */
            void Initialize();

            static ComponentManagerCollection* _componentManagerCollection; // Singleton.

            // Storage for ComponentManager data, back-end functionality, and helper functions.
            class ComponentManagerCollectionData;
            ComponentManagerCollectionData* _data = nullptr;
    };
}

#include "../src/component_manager_collection.tpp" // Template function includes.

#endif //DATASTRUCTURES_COMPONENT_MANAGER_COLLECTION_H

