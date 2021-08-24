
#ifndef SPARK_TRANSFORM_H
#define SPARK_TRANSFORM_H

#include "spark/utility.h"
#include "spark/ecs/components/types/component.h"

namespace Spark {
    namespace ECS {

        struct Transform : public IComponent {
            Transform() = default;
            ~Transform() override;
            void OnImGuiRender() override;

            bool _enabled = true;

            REGISTER_COMPONENT(Transform);
        };

    }
}

#endif //SPARK_TRANSFORM_H
