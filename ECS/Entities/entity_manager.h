
#ifndef DATASTRUCTURES_ENTITY_MANAGER_H
#define DATASTRUCTURES_ENTITY_MANAGER_H

#include <unordered_map>                             // std::unordered_map
#include "../ecs_typedefs.h"                         // EntityID
#include "../../Utilities/Logger/logging_system.h"   // LoggingSystem
#include "../../Utilities/Tools/compile_time_hash.h" // STRINGHASH

namespace ECS::Entities {
    class EntityManager {
        public:
            // Entity manager manages component system callback functions of 4 different types.
            enum class CallbackType {
                ENTITY_CREATE,
                ENTITY_DELETE,
                COMPONENT_ADD,
                COMPONENT_REMOVE
            };

            /**
             * Constructor.
             */
            EntityManager() = default;

            /**
             * Two stage initialization: initializes resources associated with the Entity Manager, including support for
             * logging and initializing entity component storage.
             */
            void Initialize();

            /**
             * Cleans up any resources associated with the Entity Manager.
             */
            ~EntityManager();

            /**
             * Function to capture a class member function to convert it to a standard callback function callable from
             * within the Entity Manager as a response to changes to entities or their components.
             * @tparam Class             - Class type which holds the member function.
             * @tparam ReturnType        - Return type of the member function.
             * @tparam FunctionArguments - Parameter types of the member function.
             * @param  classInstance     - Class 'this' pointer.
             * @param  memberFunction    - Pointer to the member function to call.
             */
            template <class Class, typename ReturnType, typename ...FunctionArguments>
            void RegisterCallback(CallbackType, Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...));

            /**
             * Create an entity. Throws error if the provided entity name matches any of the the build-in component type
             * names or any pre-existing entity names. Automatically notifies all fully registered component systems that
             * a new entity has been created.
             * @param name - Unique name for the entity to have.
             */
            void CreateEntity(std::string name);

            /**
             * Destroy an entity from the Entity Manager with the provided entity ID, given that it exists. Automatically
             * notifies all fully registered component systems that an entity has been deleted.
             * @param ID - ID of the entity to delete.
             */
            void DestroyEntity(EntityID ID);

            /**
             * Destroy an entity from the Entity Manager with the provided entity name, given that it exists. Automatically
             * notifies all fully registered component systems that an entity has been deleted.
             * @param name - Name of the entity to delete.
             */
            void DestroyEntity(std::string name);

            /**
             * Retrieve the list of components that are attached to an entity with the provided ID, given that such an
             * entity exists in the Entity Manager.
             * @param ID - ID of the entity to get the components of.
             * @return List of attached components to the entity at the provided ID.
             */
            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const;

            /**
             * Retrieve the list of components that are attached to an entity with the provided name, given that such an
             * entity exists in the Entity Manager.
             * @param name - Name of the entity to get the components of.
             * @return List of attached components to the entity at the provided name.
             */
            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const;

            /**
             * Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already
             * have a component of that type attached to it. Automatically notifies all fully registered component systems
             * that a component has been added to this entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  ID            - ID of the entity to attach component to.
             */
            template <class ComponentType>
            void AddComponent(EntityID ID);

            /**
             * Attach a component to an entity at the provided name, given that it exists and the entity doesn't already
             * have a component of that type attached to it. Automatically notifies all fully registered component systems
             * that a component has been added to this entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  name          - Name of the entity to attach component to.
             */
            template <class ComponentType>
            void AddComponent(std::string name);

            /**
             * Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager.
             * It is valid behavior to delete a component from an entity that does not manage a component of that type.
             * Automatically notifies all fully registered component systems that a component has been removed from this
             * entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  ID            - ID of the entity from which to remove the component.
             */
            template <class ComponentType>
            void DeleteComponent(EntityID ID);

            /**
             * Remove a component from an entity at the provided nae, given that the entity exists in the Entity Manager.
             * It is valid behavior to delete a component from an entity that does not manage a component of that type.
             * Automatically notifies all fully registered component systems that a component has been removed from this
             * entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  name          - Name of the entity from which to remove the component.
             */
            template <class ComponentType>
            void DeleteComponent(std::string name);

            // Entity manager should not be copied/duplicated.
            EntityManager(EntityManager const &other) = delete;
            EntityManager(EntityManager &&other) = delete;
            void operator=(EntityManager const &other) = delete;

        private:
            // Storage for EntityManager data, back-end functionality, and helper functions.
            class EntityManagerData;
            EntityManagerData* _data = nullptr;
    };
}

#include "entity_manager.tpp" // Template function includes.

#endif //DATASTRUCTURES_ENTITY_MANAGER_H

