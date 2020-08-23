
#ifndef SPARK_ENTITY_MANAGER_INTERFACE_H
#define SPARK_ENTITY_MANAGER_INTERFACE_H

#include <spark_pch.h>                                    // std::function, std::unordered_map
#include <spark/ecs/ecs_typedefs.h>                       // EntityID, ComponentTypeID
#include <spark/ecs/components/types/base_component.h>    // BaseComponent
#include <spark/ecs/entities/entity_callback_type.h>      // CallbackType
#include <spark/utilitybox/tools/global_defines.h>        // _NODISCARD_

namespace Spark {
    namespace ECS {
        namespace Entities {

            class IEntityManager {
                public:
                    virtual void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction) = 0;
                    virtual void CreateEntity(std::string name) = 0;
                    virtual void DestroyEntity(EntityID ID) = 0;
                    virtual void DestroyEntity(std::string name) = 0;
                    _NODISCARD_ virtual EntityID GetEntityIDFromName(const std::string& entityName) const = 0;
                    _NODISCARD_ virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const = 0;
                    _NODISCARD_ virtual const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const = 0;
            };

        }
    }
}

#endif // SPARK_ENTITY_MANAGER_INTERFACE_H
