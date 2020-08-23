
#ifndef SPARK_ADAPTER_H
#define SPARK_ADAPTER_H

#include <spark_pch.h>                              // std::string, std::vector
#include <spark/utilitybox/tools/global_defines.h>  // _NODISCARD
#include <spark/utilitybox/logger/adapter_config.h> // AdapterConfiguration

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class Adapter {
                public:
                    explicit Adapter(const char* name = "Unnamed Adapter");
                    virtual ~Adapter();
                    virtual void OutputMessage(std::vector<std::string> messages) = 0;

                    _NODISCARD_ AdapterConfiguration* GetConfiguration() const;
                    _NODISCARD_ int GetLogCount() const;

                protected:
                    AdapterConfiguration* _config;
                    int _logCount;
            };

        }
    }
}

#endif // SPARK_ADAPTER_H
