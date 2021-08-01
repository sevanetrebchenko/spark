
#ifndef SPARK_BASE_COMPONENT_H
#define SPARK_BASE_COMPONENT_H

#include <spark/core/rename.h> // STRINGHASH

namespace Spark {
    namespace ECS {

        struct BaseComponent {
            static constexpr unsigned ID = STRINGHASH("BaseComponent");
            static constexpr const char* Name = "Base";

            virtual ~BaseComponent() = default;
            virtual void OnImGuiRender() = 0;

            bool _enabled = true;
        };

        struct TestComponent : public BaseComponent {

            static constexpr unsigned ID = STRINGHASH("BaseComponent");
            static constexpr const char* Name = "Base";

            TestComponent() {

            }

            ~TestComponent() override {

            }

            void OnImGuiRender() override {
                //std::cout << "rendering test component imgui" << std::endl;
            }
        };

    }
}


#endif // SPARK_BASE_COMPONENT_H
