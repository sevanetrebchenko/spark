
#include <core/layer.h>

namespace Spark {

    Layer::Layer(std::string name) : _name(std::move(name)) {
        // Nothing to do here.
    }

    void Layer::OnAttach() {
        // Nothing to do here.
    }

    void Layer::OnUpdate(float dt) {
        // Nothing to do here.
    }

    void Layer::OnDetach() {
        // Nothing to do here.
    }

    void Layer::OnImGuiRender() {
        // Nothing to do here.
    }

    const std::string &Layer::GetName() const {
        return _name;
    }

}
