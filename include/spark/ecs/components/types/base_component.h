
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
                virtual void OnImGuiRender() = 0;

                bool _enabled = true;
            };

            struct TestComponent : public BaseComponent {
                void OnImGuiRender() override {
                    //std::cout << "rendering test component imgui" << std::endl;
                }
            };

        }
    }
}


#endif // SPARK_BASE_COMPONENT_H
