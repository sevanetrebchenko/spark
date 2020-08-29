
#ifndef SPARK_SHADER_DATA_TYPE_H
#define SPARK_SHADER_DATA_TYPE_H

#include <spark/core/core.h>

namespace Spark {
    namespace Graphics {

        enum class ShaderDataType {
            NONE,
            BOOL,
            INT, INT2, INT3, INT4,
            FLOAT, FLOAT2, FLOAT3, FLOAT4,
            MAT2, MAT3, MAT4,
        };

        static unsigned ShaderDataTypeSize(ShaderDataType shaderDataType);

    }
}

#endif //SPARK_SHADER_DATA_TYPE_H
