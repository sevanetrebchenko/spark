
#ifndef SPARK_IMGUI_LAYER_H
#define SPARK_IMGUI_LAYER_H

#include <core/layer.h>
#include <GLFW/glfw3.h>

namespace Spark {
    namespace Graphics {

        class OpenGLImGuiLayer : public Layer {
            public:
                explicit OpenGLImGuiLayer(GLFWwindow* window);
                ~OpenGLImGuiLayer() override;

                void OnAttach() override;
                void OnDetach() override;

                void BeginFrame();
                void EndFrame();

            private:
                class OpenGLImGuiLayerData;
                OpenGLImGuiLayerData* _data;
        };

    }
}

#endif //SPARK_IMGUI_LAYER_H
