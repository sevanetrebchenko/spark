
#ifndef DATASTRUCTURES_ENTITY_MANAGER_H
#define DATASTRUCTURES_ENTITY_MANAGER_H

#include <unordered_map>
#include "entity.h"
#include "../Components/base_component.h"
#include "../ecs_typedefs.h"

namespace ECS::Entities {
    class EntityManager {
        public:
            EntityManager();
            ~EntityManager();

            void CreateEntity(const char* name);
            void DestroyEntity(EntityID);

            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID);

            template <class ComponentType>
            ComponentType* GetComponent(EntityID ID);

            template <class ComponentType>
            void AddComponent(EntityID ID);

            template <class ComponentType>
            void DeleteComponent(EntityID ID);


            // Entity manager should not be copied.
            EntityManager(EntityManager const &other) = delete;
            EntityManager(EntityManager &&other) = delete;
            void operator=(EntityManager const &other) = delete;

        private:
            typedef std::unordered_map<ComponentTypeID, Components::BaseComponent*> _entityComponentList;
            typedef std::unordered_map<EntityID, _entityComponentList> _entityList;

            _entityList _entities;
    };
}

#endif //DATASTRUCTURES_ENTITY_MANAGER_H
