
#ifndef SPARK_VERTEX_BUFFER_LAYOUT_H
#define SPARK_VERTEX_BUFFER_LAYOUT_H

#include <spark/core/core.h>
#include <spark/graphics/renderer/shader_data_type.h>

namespace Spark {
    namespace Graphics {

        class VertexBufferElement {
            public:
                VertexBufferElement(ShaderDataType shaderDataType, std::string elementName);
                ~VertexBufferElement() = default;

                _NODISCARD_ unsigned GetComponentCount() const;

                _NODISCARD_ const std::string& GetElementName() const;
                void SetElementName(std::string elementName) const;
                _NODISCARD_ const ShaderDataType& GetShaderDataType() const;
                void SetShaderDataType(ShaderDataType shaderDataType);
                _NODISCARD_ size_t GetBufferOffset() const;
                void SetBufferOffset(unsigned bufferOffset);

            private:
                class BufferElementData;
                BufferElementData* _data;
        };

        class VertexBufferLayout {
            public:
                VertexBufferLayout();
                VertexBufferLayout(const std::initializer_list<VertexBufferElement>& bufferElements);
                ~VertexBufferLayout() = default;

                _NODISCARD_ const std::vector<VertexBufferElement>& GetBufferElements() const;
                _NODISCARD_ unsigned GetStride() const;

                std::vector<VertexBufferElement>::iterator begin();
                std::vector<VertexBufferElement>::iterator end();
                std::vector<VertexBufferElement>::const_iterator cbegin();
                std::vector<VertexBufferElement>::const_iterator cend();

            private:
                class BufferLayoutData;
                BufferLayoutData* _data;
        };

    }
}

#endif //SPARK_VERTEX_BUFFER_LAYOUT_H
