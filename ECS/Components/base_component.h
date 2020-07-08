
#ifndef DATASTRUCTURES_BASE_COMPONENT_H
#define DATASTRUCTURES_BASE_COMPONENT_H

namespace ECS::Components {
    struct BaseComponent {
        static constexpr unsigned ID = 4;
        static constexpr const char* Name = "Base";
        BaseComponent() = default;
        virtual ~BaseComponent() = default;

        bool _enabled = true;
    };
}


#endif //DATASTRUCTURES_BASE_COMPONENT_H
