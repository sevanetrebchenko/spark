
#ifndef SPARK_FILE_ADAPTER_H
#define SPARK_FILE_ADAPTER_H

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/adapter/adapter.h> // IAdapter

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class FileAdapter : public IAdapter {
                public:
                    // TODO: openmode
                    explicit FileAdapter(const char* fileName);
                    ~FileAdapter() override;
                    void Initialize() override;

                    /**
                     * Print all messages in the formatted messages buffer to the file. Separates concurrent messages with
                     * a newline to aid in clarity.
                     */
                    void OutputMessage(std::vector<std::string> messages) override;

                private:
                    const char* _fileName;
                    std::ofstream _file;
            };

        }
    }
}

#endif //SPARK_FILE_ADAPTER_H
