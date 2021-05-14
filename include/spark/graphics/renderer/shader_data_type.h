
#ifndef SPARK_SHADER_DATA_TYPE_H
#define SPARK_SHADER_DATA_TYPE_H

#include <spark/core/core.h>

namespace Spark {
    namespace Graphics {

        enum class ShaderDataType {
            NONE,
            BOOL, INT, FLOAT,
            VEC2, VEC3, VEC4,
            MAT2, MAT3, MAT4,
        };

        static std::size_t ShaderDataTypeSize(ShaderDataType shaderDataType);

    }
}

#endif //SPARK_SHADER_DATA_TYPE_H
