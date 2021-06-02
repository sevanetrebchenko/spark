
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_H
#define SPARK_COMPONENT_MANAGER_COLLECTION_H

#include <spark/ecs/components/component_manager.h> // ComponentManager

namespace Spark {
    namespace ECS {
        namespace Components {

            template <class ...ComponentTypes>
            class ComponentManagerCollection : public UtilityBox::Logger::ILoggable {
                public:
                    static ComponentManagerCollection* GetInstance();

                    template <class ComponentType>
                    ComponentManager<ComponentType>* GetComponentManager();

                private:
                    ComponentManagerCollection();
                    ~ComponentManagerCollection();

                    template <class ComponentType>
                    void CreateComponentSystem();

                    template <class ComponentType>
                    void DestroyComponentSystem();

                    std::unordered_map<ComponentTypeID, ComponentManagerInterface*> _componentManagerMap;
            };

        }
    }
}

#include <spark/ecs/components/component_manager_collection.tpp> // Template function definitions.

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_H

