
#ifndef DATASTRUCTURES_WORLD_H
#define DATASTRUCTURES_WORLD_H

#include "../ECS/Entities/entity_manager.h"
#include "../ECS/Systems/base_component_system_interface.h"
#include <vector>

namespace ENGINE_NAME {
    class World {
        public:
            static World& GetInstance();

            void Initialize();
            void Update();
            void Shutdown();

            ECS::Entities::EntityManager* GetEntityManager();

        private:
            World() = default;
            ~World() = default;

            static World* _world;

            ECS::Entities::EntityManager _entityManager {};
            std::vector<ECS::Systems::BaseComponentSystemInterface*> _systems;
    };
}


#endif //DATASTRUCTURES_WORLD_H
