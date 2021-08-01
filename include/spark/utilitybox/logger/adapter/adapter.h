
#ifndef SPARK_ADAPTER_H
#define SPARK_ADAPTER_H

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/adapter/adapter_config.h> // AdapterConfiguration

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class IAdapter {
                public:
                    explicit IAdapter(const char* name = "Unnamed IAdapter");
                    explicit IAdapter(const std::string& adapterName = "Unnamed IAdapter");
                    virtual ~IAdapter() = default;

                    virtual void Initialize() = 0;
                    virtual void OutputMessage(std::vector<std::string> messages) = 0;

                    NODISCARD AdapterConfiguration* GetConfiguration() const;
                    NODISCARD int GetLogCount() const;

                protected:
                    AdapterConfiguration* _config;
                    int _logCount;
            };

        }
    }
}

#endif // SPARK_ADAPTER_H
