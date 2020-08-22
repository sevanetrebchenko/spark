
#ifndef SPARK_ENTITY_MANAGER_INTERFACE_H
#define SPARK_ENTITY_MANAGER_INTERFACE_H

#include <ecs/entities/entity_events.h>
#include <events/event_interactable_interface.h>
#include <utilitybox/tools/utility_functions.h>

namespace Spark {
    namespace ECS {
        namespace Entities {

            template <class EventType, class ...AdditionalEventTypes>
            struct GenerateAddComponentsForTypes : AddComponentEvent<EventType>, GenerateAddComponentsForTypes<AdditionalEventTypes...> {
            };

            template <class EventType>
            struct GenerateAddComponentsForTypes<EventType> {
                using Type = AddComponentEvent<EventType>;
            };

            class EntityManagerInterface : public Events::IEventReceivable<EntityManagerInterface, GenerateAddComponentsForTypes<ALL_COMPONENTS>::Type> {
                public:
                    virtual void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction) = 0;
                    virtual void CreateEntity(std::string name) = 0;
                    virtual void DestroyEntity(EntityID ID) = 0;
                    virtual void DestroyEntity(std::string name) = 0;
                    _NODISCARD_ virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const = 0;
                    _NODISCARD_ virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const = 0;
            };

        }
    }
}

#endif //SPARK_ENTITY_MANAGER_INTERFACE_H
