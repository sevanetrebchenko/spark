
#ifndef SPARK_IMGUI_OVERHEAD_H
#define SPARK_IMGUI_OVERHEAD_H

#include <utilitybox/logger/logging_system.h> // LoggingSystem
#include <graphics/context/rendering_api.h>   // RenderingAPI

namespace Spark {
    namespace Graphics {
        namespace Context {

            class ImGuiOverhead {
                public:
                    /**
                     * Constructor.
                     * @param API - Rendering API used for underlying rendering context.
                     */
                    explicit ImGuiOverhead(RenderingAPI API);

                    /**
                     * Destructor.
                     */
                    virtual ~ImGuiOverhead();

                    /**
                     * Gets called once at the start of every frame before any ImGui code gets executed. Function sets up
                     * ImGui and platform-specific rendering for the start of a new ImGui frame.
                     */
                    virtual void StartFrame() = 0;

                    /**
                     * Gets called once at the end of the frame's ImGui segment of code. Functions draws ImGui data to
                     * the screen. No ImGui / platform-specific rendering should happen after the call to this function.
                     * Function should also always be called before the call to EndFrame().
                     */
                    virtual void RenderFrame() = 0;

                    /**
                     * Gets called once at the end of the frame's ImGui segment of code. Function wraps up ImGui and any
                     * platform-specific frame rendering in preparation for a new frame. Function should always be called
                     * last out of the three ImGuiOverhead functions.
                     */
                    virtual void EndFrame() = 0;

                protected:
                    RenderingAPI _api; // Current graphics API in use.
                    UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("ImGui Overhead - ") + std::string(APIToString(_api)) };
            };

        } // namespace Context
    } // namespace Graphics
} // namespace Spark

#endif // SPARK_IMGUI_OVERHEAD_H
