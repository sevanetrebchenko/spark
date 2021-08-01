
#ifndef SPARK_INDEX_BUFFER_OBJECT_H
#define SPARK_INDEX_BUFFER_OBJECT_H

#include <spark/core/rename.h>

namespace Spark {
    namespace Graphics {

        class IIndexBufferObject {
            public:
                static IIndexBufferObject* Create(unsigned* indices, unsigned indexCount);
                virtual ~IIndexBufferObject() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                NODISCARD virtual unsigned GetIndexCount() const = 0;
        };

    }
}

#endif //SPARK_INDEX_BUFFER_OBJECT_H
