
#ifndef SPARK_ENTITY_MANAGER_DATA_H
#define SPARK_ENTITY_MANAGER_DATA_H

#include <spark_pch.h>                                 // std::function, std::string, std::unordered_map, std::vector
#include <spark/ecs/ecs_typedefs.h>                    // EntityID, ComponentTypeID
#include <spark/ecs/components/types/base_component.h> // BaseComponent
#include <spark/utilitybox/logger/logging_interface.h> // ILoggable
#include <spark/ecs/entities/entity_callback_type.h>   // CallbackType

namespace Spark {
    namespace ECS {
        namespace Entities {

            class EntityManagerData : public UtilityBox::Logger::ILoggable {
                public:
                    /**
                    * Constructor.
                    */
                    EntityManagerData();

                    /**
                    * Cleans up any resources associated with the Entity Manager helper class.
                    */
                    ~EntityManagerData();

                    /**
                    * Create an entity. Throws error if the provided entity name matches any of the the bin-in component type
                    * names or any pre-existing entity names. Automatically notifies all fully registered component systems that
                    * a new entity has been created.
                    * @param name - Unique name for the entity to have.
                    */
                    void CreateEntity(std::string name);

                    /**
                    * Destroy an entity from the Entity Manager with the provided entity ID, given that it exists.
                    * @param ID - ID of the entity to delete.
                    */
                    void DestroyEntity(EntityID ID);

                    /**
                    * Destroy an entity from the Entity Manager with the provided entity name, given that it exists.
                    * @param name - Name of the entity to delete.
                    */
                    void DestroyEntity(std::string name);

                    /**
                     * Register a callback function of a given type with the entity manager.
                     * @param callbackType     - Callback function type.
                     * @param callbackFunction - Callback function.
                     */
                    void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction);

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

                private:
                    /**
                    * Check the name of the given entity name against the names of all of the built-in components to ensure the
                    * names do not overlap. Names are all converted to lowercase before comparison.
                    * @tparam ComponentTypes - All the built-in component types.
                    * @param name            - Entity name to be compared.
                    * @return On
                    */
                    template <class ...ComponentTypes>
                    bool CheckEntityName(const std::string& name) const;

                    /**
                    * Convert a given string to lowercase.
                    * @param string - String to convert to lowercase.
                    */
                    void ConvertToLowercase(std::string& string) const;

                    /**
                    * Convert an instance of an EntityManager::CallbackType object to a string to use in debug messages.
                    * @param callbackType - Type of callback function to convert.
                    * @return String representation of the callback function type.
                    */
                    const char* CallbackTypeToString(CallbackType callbackType);

                    // Typedef for entity component list readability.
                    typedef std::unordered_map<ComponentTypeID, Components::BaseComponent*> entityComponentList;

                    std::unordered_map<EntityID, entityComponentList> _entityComponents; // List of components attached to entities.
                    std::unordered_map<EntityID, std::string> _entityNames;              // Lsit of entity names.

                    // Typedefs for component system callback functions.
                    typedef std::function<void(EntityID)> OnEntityCreate;
                    typedef std::function<void(EntityID)> OnEntityDestroy;
                    typedef std::function<void(EntityID)> OnEntityComponentAdd;
                    typedef std::function<void(EntityID)> OnEntityComponentRemove;

                    std::vector<OnEntityCreate> _entityCreateCallbackFunctions;             // Callback functions for when entities get created.
                    std::vector<OnEntityDestroy> _entityDestroyCallbackFunctions;           // Callback functions for when entities get created.
                    std::vector<OnEntityComponentAdd> _componentAddCallbackFunctions;       // Callback functions for when entities get a component attached.
                    std::vector<OnEntityComponentRemove> _componentRemoveCallbackFunctions; // Callback functions for when entities get a component removed.
            };

        }
    }
}

#include <ecs/entities/entity_manager_data.tpp> // Template function definitions.

#endif // SPARK_ENTITY_MANAGER_DATA_H
