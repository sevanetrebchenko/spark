#include "core/core.h"

namespace Spark {
    Core* Core::_world = nullptr;

    ECS::Entities::EntityManager *Core::GetEntityManager() {
        return &_entityManager;
    }

    Core* Core::GetInstance() {
        if (!_world) {
            _world = new Core();
        }

        return _world;
    }
}
