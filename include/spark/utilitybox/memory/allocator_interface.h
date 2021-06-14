
#ifndef SPARK_ALLOCATOR_INTERFACE_H
#define SPARK_ALLOCATOR_INTERFACE_H

#include <spark/core/core.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            class IAllocator {
                public:
                    _NODISCARD_ virtual void *RetrieveBlock() = 0;
                    virtual void ReturnBlock(void *blockAddress) = 0;
            };

        }
    }
}

#endif //SPARK_ALLOCATOR_INTERFACE_H
