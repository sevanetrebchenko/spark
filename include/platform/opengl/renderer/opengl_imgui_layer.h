
#ifndef SPARK_OPENGL_IMGUI_LAYER_H
#define SPARK_OPENGL_IMGUI_LAYER_H

#include <GLFW/glfw3.h>                          // GLFWwindow
#include <spark/graphics/renderer/imgui_layer.h> // ImGuiLayer

namespace Spark {
    namespace Graphics {

        class OpenGLImGuiLayer : public IImGuiLayer {
            public:
                explicit OpenGLImGuiLayer(GLFWwindow* window);
                ~OpenGLImGuiLayer() override;

                void OnAttach() override;
                void OnDetach() override;
                void OnUpdate(float dt) override;
                void OnImGuiRender() override;

                void BeginFrame() override;
                void EndFrame() override;

            private:
                class OpenGLImGuiLayerData;
                OpenGLImGuiLayerData* _data;
        };

    }
}

#endif // SPARK_OPENGL_IMGUI_LAYER_H
