
#ifndef SPARK_FILE_ADAPTER_H
#define SPARK_FILE_ADAPTER_H

#include "spark/core/utility.h"
#include "spark/utilitybox/logger/adapter/adapter.h"

namespace Spark {
    namespace UtilityBox {

        class FileAdapter : public IAdapter {
            public:
                explicit FileAdapter(const std::string& outputFilename, const AdapterConfiguration& configuration);
                ~FileAdapter() override;

            private:
                void OutputMessage(const std::string& message) override;

                std::ofstream output_;
        };

    }
}

#endif //SPARK_FILE_ADAPTER_H
