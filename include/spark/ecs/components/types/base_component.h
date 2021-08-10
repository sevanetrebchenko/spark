
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include "spark/ecs/utility.h"

namespace Spark {
    namespace ECS {

        struct IComponent {
            virtual ~IComponent() = default;
            virtual void OnImGuiRender() = 0;

            bool _enabled = true;

            REGISTER_COMPONENT(IComponent)
        };

    }
}


#endif // SPARK_BASE_COMPONENT_H
