
#ifndef SPARK_COMPONENT_MANAGER_COLLECTION_H
#define SPARK_COMPONENT_MANAGER_COLLECTION_H

#include <spark/ecs/components/component_manager.h> // ComponentManager
#include <spark/utilitybox/tools/singleton.h>

namespace Spark {
    namespace ECS {

        template <class ...ComponentTypes>
        class ComponentManagerCollection : public Singleton<ComponentManagerCollection<ComponentTypes...>>, public UtilityBox::Logger::ILoggable {
            public:
                REGISTER_SINGLETON(ComponentManagerCollection<ComponentTypes...>);

                template <class ComponentType>
                ComponentManager<ComponentType>* GetComponentManager();

            protected:
                ComponentManagerCollection();
                ~ComponentManagerCollection() override;

            private:
                template <class ComponentType>
                void CreateComponentSystem();

                template <class ComponentType>
                void DestroyComponentSystem();

                std::unordered_map<ComponentTypeID, IComponentManager*> componentManagerMap_;
        };

    }
}

#include <spark/ecs/components/component_manager_collection.tpp> // Template function definitions.

#endif // SPARK_COMPONENT_MANAGER_COLLECTION_H

