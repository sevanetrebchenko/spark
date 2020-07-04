
#ifndef DATASTRUCTURES_WORLD_H
#define DATASTRUCTURES_WORLD_H

#include "../ECS/Components/component_manager_collection.h"
#include "../ECS/Entities/entity_manager.h"
//#include "../ECS/Systems/base_component_system.h"

#define ALL_COMPONENTS

namespace ENGINE_NAME {
    class World {
        public:
            static World& GetInstance();

            void Initialize();
            void Update();
            void Shutdown();

            ECS::Components::ComponentManagerCollection<ALL_COMPONENTS>* GetComponentManagerCollection();
            ECS::Entities::EntityManager* GetEntityManager();

        private:
            World();
            ~World();

            static World* _world;

            ECS::Components::ComponentManagerCollection<ALL_COMPONENTS> _componentManagerCollection {};
            ECS::Entities::EntityManager _entityManager {};
            //std::vector<ECS::Systems::BaseComponentSystemInterface*> _systems;
    };
}




#endif //DATASTRUCTURES_WORLD_H
