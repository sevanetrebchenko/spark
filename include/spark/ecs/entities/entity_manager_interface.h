
#ifndef SPARK_ENTITY_MANAGER_INTERFACE_H
#define SPARK_ENTITY_MANAGER_INTERFACE_H

namespace Spark {
    namespace ECS {
        namespace Entities {

            class EntityManagerInterface {
                public:
                    virtual void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction);
                    virtual void CreateEntity(std::string name);
                    virtual void DestroyEntity(EntityID ID);
                    virtual void DestroyEntity(std::string name);
                    virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const;
                    virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const;

                    template <class ComponentType>
                    virtual void AddComponent(EntityID ID);

                    template <class ComponentType>
                    virtual void AddComponent(std::string name);

                    template <class ComponentType>
                    virtual void DeleteComponent(EntityID ID);

                    template <class ComponentType>
                    virtual void DeleteComponent(std::string name);
            };

        }
    }
}

#endif //SPARK_ENTITY_MANAGER_INTERFACE_H
