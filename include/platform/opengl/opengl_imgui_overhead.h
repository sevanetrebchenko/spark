
#ifndef SPARK_OPENGL_IMGUI_OVERHEAD_H
#define SPARK_OPENGL_IMGUI_OVERHEAD_H

#include <graphics/context/imgui_overhead.h> // ImGuiOverhead
#include <graphics/context/window.h>         // Window

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLImGuiOverhead : public Graphics::Context::ImGuiOverhead {
                public:
                    /**
                     * Initialize ImGui for OpenGL given a fully initialized window.
                     * @param window      - Initialized window to create ImGui context in.
                     */
                    explicit OpenGLImGuiOverhead(Graphics::Context::Window* window);

                    /**
                     * Cleans up memory associated with OpenGL implementation of ImGui overhead.
                     */
                    ~OpenGLImGuiOverhead() override;

                    /**
                     * Gets called once at the start of every frame before any ImGui code gets executed. Function updates
                     * ImGui mouse buttons and position and updates ImGui framebuffer in case the underlying window was
                     * resized.
                     * Notice: This function should be called every frame before any ImGui code is called.
                     */
                    void StartFrame() override;

                    /**
                     * Gets called once at the end of the frame's ImGui segment of code. Functions draws all ImGui-related
                     * data to the screen.
                     * Notice: This function should be called every frame:
                     *              - before EndFrame()
                     *              - after all ImGui code is called
                     *
                     */
                    void RenderFrame() override;

                    /**
                     * Terminate the ImGui frame.
                     * Notice: This function should be called every frame after RenderFrame() is called.
                     */
                    void EndFrame() override;

                private:
                    // Storage for ImGuiOverhead data, back-end functionality, and helper functions.
                    class ImGuiOverheadData;
                    ImGuiOverheadData* _data;
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif // SPARK_OPENGL_IMGUI_OVERHEAD_H
