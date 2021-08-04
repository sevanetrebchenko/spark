
#ifndef SPARK_ENTITY_MANAGER_TPP
#define SPARK_ENTITY_MANAGER_TPP

namespace Spark::ECS {

    template<class ComponentType>
    void EntityManager::AddComponent(EntityID ID) {
        EntityComponentMap& componentMap = entityComponents_[ID]; // Constructs if entity does not yet exist.

        ComponentManager<ComponentType>* componentManager = Singleton<ComponentManagerCollection<ALL_COMPONENTS>>::GetInstance()->GetComponentManager<ComponentType>();
        ComponentType* component = componentManager->CreateComponent();

        auto componentIter = componentMap.find(ComponentType::ID);
        if (componentIter == componentMap.end()) {
            componentMap.insert({ ComponentType::ID, component });
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
            LogWarning("Component remove failure: Entity %u does not exist.", ID);
            return;
        }

        ComponentManager<ComponentType>* componentManager = Singleton<ComponentManagerCollection<ALL_COMPONENTS>>::GetInstance()->template GetComponentManager<ComponentType>();
        componentManager->DeleteComponent(componentIter->second);
    }

    template<class... ComponentTypes>
    bool EntityManager::EntityNameMatchesComponentName(const std::string &entityName) const {
        return (StringCompare(entityName, std::string(ComponentTypes::Name)) || ...);
    }

}

#endif // SPARK_ENTITY_MANAGER_TPP