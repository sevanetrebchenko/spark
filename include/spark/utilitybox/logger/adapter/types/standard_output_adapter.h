
#ifndef SPARK_STANDARD_OUTPUT_ADAPTER_H
#define SPARK_STANDARD_OUTPUT_ADAPTER_H

#include "spark/core/utility.h"
#include "spark/utilitybox/logger/adapter/adapter.h"

namespace Spark {
    namespace UtilityBox {

        class StandardOutputAdapter : public IAdapter {
            public:
                explicit StandardOutputAdapter(std::ostream& outputStream, const AdapterConfiguration& configuration);
                ~StandardOutputAdapter() override;

            private:
                void OutputMessage(const std::string& message) override;
                std::ostream& output_;
        };

    }
}

#endif //SPARK_STANDARD_OUTPUT_ADAPTER_H
