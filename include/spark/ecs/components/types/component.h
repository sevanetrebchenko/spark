
#ifndef SPARK_COMPONENT_H
#define SPARK_COMPONENT_H

namespace Spark {
    namespace ECS {

        struct IComponent {
            virtual ~IComponent() = default;
            virtual void OnImGuiRender() = 0;

            bool enabled_ = true;
        };

    }
}


#endif // SPARK_COMPONENT_H
