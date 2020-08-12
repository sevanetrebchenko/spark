
#ifndef SPARK_ENTRY_POINT_H
#define SPARK_ENTRY_POINT_H

#include <core/application.h>

namespace Spark {
    extern Application* CreateApplication() {
        return new Application();
    }
}
#endif // SPARK_ENTRY_POINT_H
