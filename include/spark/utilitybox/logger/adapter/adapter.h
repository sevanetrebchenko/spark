
#ifndef SPARK_ADAPTER_H
#define SPARK_ADAPTER_H

#include "spark/utilitybox/logger/log_record.h"
#include "spark/utilitybox/logger/adapter/adapter_configuration.h"

namespace Spark {
    namespace UtilityBox {

        class IAdapter {
            public:
                explicit IAdapter(const AdapterConfiguration& configuration);
                virtual ~IAdapter() = default;

                void OutputMessages(const std::vector<std::string>& messages);

                NODISCARD const AdapterConfiguration& GetAdapterConfiguration() const;
                NODISCARD const std::string& GetName() const;

            protected:
                virtual void OutputMessage(const std::string& message) = 0;

            private:
                AdapterConfiguration configuration_;
        };

    }
}

#endif //SPARK_ADAPTER_H
