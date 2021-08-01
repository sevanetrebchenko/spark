
#ifndef SPARK_LOGGING_INTERFACE_H
#define SPARK_LOGGING_INTERFACE_H

#include <spark/core/rename.h>

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class ILoggable {
                public:
                    void LogDebug(const char* formatString, ...) const;
                    void LogWarning(const char* formatString, ...) const;
                    void LogError(const char* formatString, ...) const;

                protected:
                    // Make this class abstract.
                    explicit ILoggable(std::string systemName = "Not provided.");
                    ~ILoggable();

                private:
                    class ILoggableData;
                    ILoggableData* _data;
            };

        }
    }
}

#endif // SPARK_LOGGING_INTERFACE_H
