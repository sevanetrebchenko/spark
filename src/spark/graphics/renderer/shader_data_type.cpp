
#include <spark/graphics/renderer/shader_data_type.h>

namespace Spark::Graphics {

    unsigned ShaderDataTypeSize(ShaderDataType shaderDataType) {
        switch (shaderDataType) {
            case ShaderDataType::NONE:
                return 0;
            case ShaderDataType::FLOAT:
                return sizeof(float);
            case ShaderDataType::FLOAT2:
                return sizeof(float) * 2;
            case ShaderDataType::FLOAT3:
                return sizeof(float) * 3;
            case ShaderDataType::FLOAT4:
                return sizeof(float) * 4;
            case ShaderDataType::MAT2:
                return sizeof(float) * 2 * 2;
            case ShaderDataType::MAT3:
                return sizeof(float) * 3 * 3;
            case ShaderDataType::MAT4:
                return sizeof(float) * 4 * 4;
            case ShaderDataType::INT:
                return sizeof(int);
            case ShaderDataType::INT2:
                return sizeof(int) * 2;
            case ShaderDataType::INT3:
                return sizeof(int) * 3;
            case ShaderDataType::INT4:
                return sizeof(int) * 4;
            case ShaderDataType::BOOL:
                return sizeof(bool);
            default:
                SP_ASSERT(false, "ShaderDataType with ID: %i is not a valid shader data type.", shaderDataType);
        }
    }

}

