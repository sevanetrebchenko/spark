
#ifndef SPARK_WORLD_H
#define SPARK_WORLD_H

#include "../../include/spark/ecs/entities/entity_manager.h"
#include "../../include/spark/ecs/systems/base_component_system_interface.h"
#include "../../include/spark/spark_pch.h" // vector

namespace Spark {
    class World {
        public:
            static World& GetInstance();

            void Initialize();
            void Update();
            void Shutdown();

            Spark::ECS::Entities::EntityManager* GetEntityManager();

        private:
            World() = default;
            ~World() = default;

            static World* _world;

            Spark::ECS::Entities::EntityManager _entityManager {};
            std::vector<Spark::ECS::Systems::BaseComponentSystemInterface*> _systems;
    };
}


#endif // SPARK_WORLD_H
