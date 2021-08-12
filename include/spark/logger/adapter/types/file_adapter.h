
#ifndef SPARK_FILE_ADAPTER_H
#define SPARK_FILE_ADAPTER_H

#include "spark/utility.h"
#include "spark/logger/adapter/adapter.h"

namespace Spark {
    namespace Logger {

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
