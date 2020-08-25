
#include <sandbox.h>
#include <sandbox_layer.h>

Sandbox::Sandbox() : Spark::Application() {
    PushLayer(new SandboxLayer());
}

Spark::Application* Spark::CreateApplication() {
    return new Sandbox();
}
