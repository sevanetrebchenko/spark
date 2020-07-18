
#ifndef SPARK_CORE_H
#define SPARK_CORE_H

#include "ecs/entities/entity_manager.h"
#include "ecs/systems/base_component_system_interface.h"
#include <spark_pch.h> // vector

namespace Spark {
    class Core {
        public:
            static Core* GetInstance();

            void Initialize();
            void Update();
            void Shutdown();

            Spark::ECS::Entities::EntityManager* GetEntityManager();

        private:
            Core() = default;
            ~Core() = default;

            static Core* _world;

            Spark::ECS::Entities::EntityManager _entityManager {};
            std::vector<Spark::ECS::Systems::BaseComponentSystemInterface*> _systems;
    };
}


#endif // SPARK_CORE_H
