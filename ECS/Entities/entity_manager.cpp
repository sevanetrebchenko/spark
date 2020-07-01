
#include "entity_manager.h"
#include <algorithm>

namespace ECS::Entities {
    EntityManager::EntityManager() {
    }

    void EntityManager::CreateEntity(const char *name) {
    }

    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(EntityID ID) {
        return _entities.at(ID);
    }

    void EntityManager::RegisterCallback(void (*callbackFunction)(ECS::EntityID), EntityManager::CallbackType callbackFunctionType) {
        switch (callbackFunctionType) {
            case CallbackType::ENTITY_CREATE:
                _entityCreateCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::ENTITY_DELETE:
                _entityDestroyCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::COMPONENT_ADD:
                _componentAddCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::COMPONENT_REMOVE:
                _componentRemoveCallbackFunctions.emplace_back(callbackFunction);
                break;
        }
    }
}


