
#ifndef SPARK_RENDERING_API_H
#define SPARK_RENDERING_API_H

namespace Spark {
    namespace Graphics {
        namespace Context {

            enum class RenderingAPI {
                OPENGL
            };

            /**
             * Convert rendering API selection to a human-readable string.
             * @param API - API name to convert.
             * @return String representation of API name.
             */
            const char* APIToString(RenderingAPI API);

        }
    }
}

#endif //SPARK_RENDERING_API_H
