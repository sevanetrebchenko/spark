
#ifndef SPARK_STANDARD_OUTPUT_ADAPTER_H
#define SPARK_STANDARD_OUTPUT_ADAPTER_H

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/adapter/adapter.h> // IAdapter

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            //------------------------------------------------------------------------------------------------------------------
            // STANDARD ADAPTER
            //------------------------------------------------------------------------------------------------------------------
            class StandardOutputAdapter final : public IAdapter {
            public:
                /**
                 * Construct an adapter for a standard output stream.
                 * @param name   - Name of the adapter.
                 * @param stream - Stream to print to.
                 */
                explicit StandardOutputAdapter(const char* name, std::ostream* stream);

                /**
                 * Clear formatted messages and flush remaining messages into the stream.
                 */
                ~StandardOutputAdapter() override;

                void Initialize() override;

                /**
                 * Print all messages in the formatted messages buffer to the stream. Separates concurrent messages with
                 * a newline to aid in clarity.
                 */
                void OutputMessage(std::vector<std::string> messages) override;

            private:
                std::ostream* _stream;
            };

        }
    }
}

#endif //SPARK_STANDARD_OUTPUT_ADAPTER_H
