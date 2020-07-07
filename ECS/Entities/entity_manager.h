
#ifndef DATASTRUCTURES_ENTITY_MANAGER_H
#define DATASTRUCTURES_ENTITY_MANAGER_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <cstdint>
#include "entity.h"
#include "../Components/base_component.h"
#include "../ecs_typedefs.h"
#include "../../Utilities/Logger/logging_system.h"
#include "../../Utilities/Tools/compile_time_hash.h"

namespace ECS::Entities {
    class EntityManager {
        public:
            enum class CallbackType {
                ENTITY_CREATE,
                ENTITY_DELETE,
                COMPONENT_ADD,
                COMPONENT_REMOVE
            };

            EntityManager();
            ~EntityManager();

            void CreateEntity(const char* name);
            void DestroyEntity(EntityID);

            EntityID GetEntityID();

            template <class Class, typename ReturnType, typename ...FunctionArguments>
            void RegisterCallback(CallbackType, Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...));

            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID);

            template <class ComponentType>
            void AddComponent(EntityID ID);

            template <class ComponentType>
            void DeleteComponent(EntityID ID);

            // Entity manager should not be copied.
            EntityManager(EntityManager const &other) = delete;
            EntityManager(EntityManager &&other) = delete;
            void operator=(EntityManager const &other) = delete;

        private:
            UtilityBox::Logger::LoggingSystem _loggingSystem { "Entity Manager" };

            template <class Class, typename ReturnType, typename ...Args>
            auto GetMemberFunction(Class* classInstance, ReturnType(Class::*memberFunction)(Args...));

            typedef std::unordered_map<ComponentTypeID, Components::BaseComponent*> _entityComponentList;
            typedef std::unordered_map<EntityID, _entityComponentList> _entityList;

            _entityList _entities;

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

#include "entity_manager.tpp" // Template function includes.

#endif //DATASTRUCTURES_ENTITY_MANAGER_H

