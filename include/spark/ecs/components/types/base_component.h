
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include <spark/core/core.h> // STRINGHASH

namespace Spark {
    namespace ECS {
        namespace Components {

            struct BaseComponent {
                static constexpr unsigned ID = STRINGHASH("BaseComponent");
                static constexpr const char* Name = "Base";

                virtual ~BaseComponent() = default;
                bool _enabled = true;
            };

        }
    }
}


#endif // SPARK_BASE_COMPONENT_H
