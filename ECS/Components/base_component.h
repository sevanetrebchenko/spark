
#ifndef DATASTRUCTURES_BASE_COMPONENT_H
#define DATASTRUCTURES_BASE_COMPONENT_H

namespace ECS::Components {
    struct BaseComponent {
        BaseComponent();
        virtual ~BaseComponent() = default;

        bool _enabled = true;
    };
}


#endif //DATASTRUCTURES_BASE_COMPONENT_H
