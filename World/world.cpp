#include "world.h"

namespace ENGINE_NAME {
    World* World::_world = nullptr;

    ECS::Entities::EntityManager *World::GetEntityManager() {
        return &_entityManager;
    }

    World &World::GetInstance() {
        if (!_world) {
            _world = new World();
            _world->Initialize();
        }

        return *_world;
    }

    void World::Initialize() {
        _entityManager.Initialize();
    }
}
