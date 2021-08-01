
#ifndef SPARK_VERTEX_BUFFER_LAYOUT_H
#define SPARK_VERTEX_BUFFER_LAYOUT_H

#include <spark/core/rename.h>
#include <spark/graphics/renderer/shader_data_type.h>

namespace Spark {
    namespace Graphics {

        class VertexBufferElement {
            public:
                VertexBufferElement(ShaderDataType shaderDataType, std::string elementName);
                ~VertexBufferElement() = default;

                NODISCARD unsigned GetComponentCount() const;

                NODISCARD const std::string& GetElementName() const;
                void SetElementName(std::string elementName) const;
                NODISCARD const ShaderDataType& GetShaderDataType() const;
                void SetShaderDataType(ShaderDataType shaderDataType);
                NODISCARD size_t GetBufferOffset() const;
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

                NODISCARD const std::vector<VertexBufferElement>& GetBufferElements() const;
                NODISCARD unsigned GetStride() const;

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
