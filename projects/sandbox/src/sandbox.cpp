
#include <sandbox.h>
#include <sandbox_layer.h>

Sandbox::Sandbox() : Spark::Application() {
}

Spark::Application* Spark::CreateApplication() {
    return new Sandbox();
}
