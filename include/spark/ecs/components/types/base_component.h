
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include <spark/core/rename.h> // STRINGHASH
#include "../../utility.h"

namespace Spark {
    namespace ECS {

        struct BaseComponent {
            virtual ~BaseComponent() = default;
            virtual void OnImGuiRender() = 0;

            bool _enabled = true;

            REGISTER_COMPONENT(BaseComponent)
        };

        struct TestComponent : public BaseComponent {

            TestComponent() {

            }

            ~TestComponent() override {

            }

            void OnImGuiRender() override {
                //std::cout << "rendering test component imgui" << std::endl;
            }

            REGISTER_COMPONENT(TestComponent)
        };

    }
}


#endif // SPARK_BASE_COMPONENT_H
