
#ifndef SPARK_ENTITY_MANAGER_H
#define SPARK_ENTITY_MANAGER_H

#include <spark_pch.h>                                        // std::unordered_map
#include <ecs/entities/entity_callback_type.h>                // CallbackType
#include <ecs/ecs_typedefs.h>                                 // EntityID, ComponentTypeID
#include <ecs/components/base_component.h>                     // BaseComponent
#include "../../../../src/ecs/entities/entity_manager_data.h" // EntityManagerData

namespace Spark {
    namespace ECS {
        namespace Entities {

            class EntityManager {
                public:
                    /**
                    * Constructor.
                    */
                    EntityManager();

                    /**
                    * Cleans up any resources associated with the Entity Manager.
                    */
                    ~EntityManager();

                    /**
                     * Register a callback function of a given type with the entity manager.
                     * @param callbackType     - Callback function type.
                     * @param callbackFunction - Callback function.
                     */
                    void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction);

                    /**
                    * Create an entity. Throws error if the provided entity name matches any of the the bin-in component type
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
                    EntityManagerData* _data = nullptr;
            };

        } // namespace Entities
    } // namespace ECS
} // namespace Spark

#include "../../../../src/ecs/entities//entity_manager.tpp" // Template function includes.

#endif // SPARK_ENTITY_MANAGER_H


