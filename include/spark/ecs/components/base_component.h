
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include "utilitybox/tools/compile_time_hash.h" // STRINGHASH

namespace Spark {
    namespace ECS {
        namespace Components {

            struct BaseComponent {
                static constexpr unsigned ID = STRINGHASH("BaseComponent");
                static constexpr const char* Name = "Base";

                virtual ~BaseComponent() = default;
                bool _enabled = true;
            };

        } // namespace Components
    } // namespace ECS
} // namespace Spark


#endif // SPARK_BASE_COMPONENT_H
