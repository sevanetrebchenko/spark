
#ifndef SPARK_ENTITY_MANAGER_TPP
#define SPARK_ENTITY_MANAGER_TPP

#include <spark/events/event_hub.h>

namespace Spark::ECS {

    template<class ComponentType>
    void EntityManager::AddComponent(EntityID ID) {
        EntityComponentMap& componentMap = entityComponents_[ID]; // Constructs if entity does not yet exist.

        ComponentManager<ComponentType>* componentManager = Singleton<ComponentManagerCollection<COMPONENT_TYPES>>::GetInstance()->GetComponentManager<ComponentType>();
        ComponentType* component = componentManager->CreateComponent();

        auto componentIter = componentMap.find(ComponentType::ID);
        if (componentIter == componentMap.end()) {
            componentMap.insert({ ComponentType::ID, component });
            Singleton<Events::EventHub>::GetInstance()->Dispatch(new Events::RefreshObjectComponentListEvent(ID)); // Dispatch filtering event for component systems.
        }
        else {
            LogWarning("Component add failure: Entity %u already has an instance of component of type: '%s'", ID, ComponentType::Name);
        }
    }

    template<class ComponentType>
    void EntityManager::RemoveComponent(EntityID ID) {
        // Find entity.
        auto componentMapIter = entityComponents_.find(ID);
        if (componentMapIter == entityComponents_.end()) {
            LogWarning("Component remove failure: Entity %u does not exist.", ID);
            return;
        }

        // Find component.
        EntityComponentMap& componentMap = componentMapIter->second;
        auto componentIter = componentMap.find(ComponentType::ID);
        if (componentIter == componentMap.end()) {
            LogWarning("Component remove failure: Component of type %u does not exist.", ComponentType::ID); // TODO
            return;
        }

        ComponentManager<ComponentType>* componentManager = Singleton<ComponentManagerCollection<COMPONENT_TYPES>>::GetInstance()->template GetComponentManager<ComponentType>();
        componentManager->DeleteComponent(static_cast<TestComponent *>(componentIter->second));
        Singleton<Events::EventHub>::GetInstance()->Dispatch(new Events::RefreshObjectComponentListEvent(ID)); // Dispatch filtering event for component systems.
    }

    template<class... ComponentTypes>
    bool EntityManager::EntityNameMatchesComponentName(const std::string &entityName) const {
        return (StringCompare(entityName, std::string(ComponentTypes::Name)) || ...);
    }

}

#endif // SPARK_ENTITY_MANAGER_TPP