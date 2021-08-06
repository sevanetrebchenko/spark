
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include "spark/ecs/utility.h"

namespace Spark {
    namespace ECS {

        struct BaseComponent {
            virtual ~BaseComponent() = default;
            virtual void OnImGuiRender() = 0;

            bool _enabled = true;

            REGISTER_COMPONENT(BaseComponent)
        };

    }
}


#endif // SPARK_BASE_COMPONENT_H
