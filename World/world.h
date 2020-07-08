
#ifndef DATASTRUCTURES_WORLD_H
#define DATASTRUCTURES_WORLD_H

#include "../ECS/Components/component_manager_collection.h"
#include "../ECS/Entities/entity_manager.h"
#include "../ECS/Systems/base_component_system_interface.h"
#include "all_components.h"

namespace ENGINE_NAME {
    class World {
        public:
            static World& GetInstance() {
                if (!_world) {
                    _world = new World();
                }

                return *_world;
            };

            void Initialize();
            void Update();
            void Shutdown();

            ECS::Components::ComponentManagerCollection<ALL_COMPONENTS>* GetComponentManagerCollection() {
                return &_componentManagerCollection;
            };

            ECS::Entities::EntityManager* GetEntityManager() {
                return &_entityManager;
            }

        private:
            World() = default;
            ~World() = default;

            static World* _world;

            ECS::Components::ComponentManagerCollection<ALL_COMPONENTS> _componentManagerCollection {};
            ECS::Entities::EntityManager _entityManager {};
            std::vector<ECS::Systems::BaseComponentSystemInterface*> _systems;
    };
}




#endif //DATASTRUCTURES_WORLD_H
