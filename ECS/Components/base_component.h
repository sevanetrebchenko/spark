
#ifndef DATASTRUCTURES_BASE_COMPONENT_H
#define DATASTRUCTURES_BASE_COMPONENT_H

namespace ECS::Components {
    struct BaseComponent {
        BaseComponent();
        virtual ~BaseComponent();
        bool _enabled;
    };
}


#endif //DATASTRUCTURES_BASE_COMPONENT_H
