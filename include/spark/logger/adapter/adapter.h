
#ifndef SPARK_ADAPTER_H
#define SPARK_ADAPTER_H

#include "spark/logger/log_record.h"
#include "spark/logger/adapter/adapter_configuration.h"

namespace Spark {
    namespace Logger {

        class IAdapter {
            public:
                explicit IAdapter(AdapterConfiguration  configuration);
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
