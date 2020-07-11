
#include "logger.h"         // Logger functions
#include "../Tools/assert_dev.h"  // Asserts
#include <stdexcept>        // std::invalid_argument
#include <mutex>            // std::mutex
#include <thread>           // std::thread
#include <atomic>           // std::atomic
#include <ostream>          // std::cout, std::cerr
#include <algorithm>        // std::find
#include <cstdlib>          // atexit

namespace UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // STANDARD ADAPTER
    //------------------------------------------------------------------------------------------------------------------
    class StandardOutputAdapter final : public Adapter {
        public:
            /**
             * Construct an adapter for a standard output stream.
             * @param name   - Name of the adapter.
             * @param stream - Stream to print to.
             */
            explicit StandardOutputAdapter(std::string&& name, std::ostream* stream);

            /**
             * Clear formatted messages and flush remaining messages into the stream.
             */
            ~StandardOutputAdapter() override;

            /**
             * Print all messages in the formatted messages buffer to the stream. Separates concurrent messages with
             * a newline to aid in clarity.
             */
            void OutputMessage() override;

        private:
            std::ostream* _stream; // Standard output stream this adapter prints to.
    };

    // Construct an adapter for a standard output stream.
    StandardOutputAdapter::StandardOutputAdapter(std::string&& name, std::ostream* stream) : Adapter(std::move(name)), _stream(stream) {
        // Nothing to do here.
    }

    // Print all messages in the formatted messages buffer to the stream. Separates concurrent messages with a newline to aid in clarity.
    void StandardOutputAdapter::OutputMessage() {
        ++_logCount;

        // Print out all messages.
        while (!_formattedMessages.empty()) {
            const std::string& message = _formattedMessages.front();
            (*_stream) << message;
            _formattedMessages.pop();
        }

        // Append this message with another newline character to better separate concurrent messages.
        (*_stream) << std::endl;
    }

    // Clear formatted messages and flush remaining messages into the stream.
    StandardOutputAdapter::~StandardOutputAdapter() {
        OutputMessage();
        (*_stream).flush();
    }


    //------------------------------------------------------------------------------------------------------------------
    // PROCESSING ADAPTER
    //------------------------------------------------------------------------------------------------------------------
    class ProcessingAdapter : public Adapter {
        public:
            /**
             * Construct an adapter for intermittent message processing.
             * @param name - Name of the adapter.
             */
            explicit ProcessingAdapter(std::string&& name);

            /**
             * Required overload from base class - does nothing.
             */
            void OutputMessage() override;

            /**
             * Clear messages in the formatting store.
             */
            void ClearMessages();

            /**
             * Set the log count to use during formatting of the next message.
             * @param logCount
             */
            void SetLogCount(const int& logCount);

            /**
             * Get the current store of formatted messages.
             * @return Reference to the queue of current completely formatted messages.
             */
            std::queue<std::string>& GetFormattedMessages();
    };

    // Construct an adapter for intermittent message processing.
    ProcessingAdapter::ProcessingAdapter(std::string&& name) : Adapter(std::move(name)) {
        // Nothing to do here.
    }

    // Required overload from base class - does nothing.
    void ProcessingAdapter::OutputMessage() {
        // Nothing to do here.
    }

    // Clear messages in the formatting store.
    void ProcessingAdapter::ClearMessages() {
        while (!_formattedMessages.empty()) {
            _formattedMessages.pop();
        }
    }

    // Set the log count to use during formatting of the next message.
    void ProcessingAdapter::SetLogCount(const int& logCount) {
        _logCount = logCount;
    }

    // Get the current store of formatted messages.
    std::queue<std::string>& ProcessingAdapter::GetFormattedMessages() {
        return _formattedMessages;
    }


    //------------------------------------------------------------------------------------------------------------------
    // LOGGING HUB DATA
    //------------------------------------------------------------------------------------------------------------------
    class LoggingHub::LoggingHubData {
        public:
            /**
             * Initialize data for the LoggingHub to function properly. Starts an asynchronous worker thread working
             * on the DistributeMessages function at a fixed interval.
             * @param initTimestamp - Initialization timestamp of the parent LoggingHub.
             */
            explicit LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp);

            /**
             * Clean up the data related to the LoggingHub.
             */
            ~LoggingHubData();

            /**
             * Safely terminate the worker thread. Ensures all remaining messages will be outputted.
             */
            void TerminateWorkerThread();

            /**
             * Add a custom adapter to receive messages.
             * @param adapter - Adapter instance to add.
             */
            void AttachAdapter(Adapter* adapter);

            /**
             * Remove a custom adapter from receiving messages. Calls destructor for the adapter.
             * @param adapter - Adapter instance to remove.
             */
            void DetachAdapter(Adapter* adapter);

            /**
             * Get an adapter instance with a given name.
             * @param name - Name of the desired adapter.
             * @return On success: returns a pointer to the adapter.
             *         On failure: returns a null pointer.
             */
            Adapter* GetAdapter(const std::string& name);

            /**
             * Used to verify the message pointer (provided to adapters' ProcessMessage function) before allowing access
             * to internal message data. Safeguard against pointer modification and random data requests from sources
             * that do not have access permissions to log message internals.
             * @param messageAddress - Pointer (provided to adapters' ProcessMessage function).
             * @return On verification success: true (valid pointer).
             *         On verification failure: false (invalid pointer).
             */
            bool VerifyDataPointer(void* messageAddress);

            /**
             * Transfer (move) log message data into internal data stores in the LoggingHub.
             * @param records         - Reference to the internal message storage for log records of the message - data will be moved entirely from this message.
             * @param messageSeverity - Severity of the passed message.
             * @param systemName      - System name of the logging system that logged this message.
             */
            void Emplace(std::vector<LogMessage::LogRecord>& records, const LogMessageSeverity& messageSeverity, const std::string& systemName);

            /**
             * Given a message pointer, safely get the log records of the underlying message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the message store.
             */
            const std::vector<LogMessage::LogRecord>& GetLogRecords(void* messageAddress);

            /**
             * Given a message pointer, safely get the message severity of the underlying message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the message severity.
             */
            const LogMessageSeverity& GetMessageSeverity(void* messageAddress);

            /**
             * Given a message pointer, safely get the name of the logging system that logged the message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the logging system's name.
             */
            const std::string& GetThroughLoggingSystem(void* messageAddress);

            /**
             * Get the initialization timestamp of the LoggingHub.
             *
             * @return Timestamp of when the LoggingHub was initialized.
             */
            const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingHubInitializationTimestamp();

        private:
            /**
             * Run by the worker thread to process and distribute message to adapters.
             */
            void DistributeMessages();

            /**
             * Switch the 'printed-to' buffer to be able to safely distribute the other buffer of stored message data.
             */
            void SwitchBuffers();

            /**
             * Constructs the components of a message that is sent to the Standard Error adapter in the occurrence of an
             * exception during message processing by custom adapters.
             * @param exception   - Caught exception.
             * @param adapterName - Name of the custom adapter that failed to process the message safely.
             */
            void ConstructStandardErrorMessage(const std::invalid_argument& exception, const char* adapterName);

            /**
             * Constructs the components of a message that is sent to the custom adapter in the occurrence of an
             * exception during message processing.
             * @param exception   - Caught exception.
             * @param adapterName - Name of the custom adapter that failed to process the message safely.
             */
            void ConstructAdapterErrorMessage(const std::invalid_argument& exception, const char* adapterName);

            /**
             * Process the underlying message (provided by messageAddress) following the desired header and message formats.
             * Stores the resulting processed error messages in the errorProcessingAdapter's formatted message store.
             * @param headerFormat   - Desired header format for the message.
             * @param messageFormat  - Desired message format for the message.
             * @param messageAddress - Address of the message data to be used during processing.
             */
            void ProcessMessageWithFormat(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat, void* messageAddress);

            /**
             * Provided a format to follow, construct the header of an error message. This function can be used
             * multiple times in the same error message.
             * @param headerFormat - Desired format of the header.
             */
            void ConstructErrorMessageHeader(const std::queue<HeaderFormatElement>& headerFormat);

            /**
             * Provided a format to follow, construct a body component of an error message. This function can be used
             * multiple times in the same error message.
             * @param messageFormat - Desired format of the component.
             */
            void ConstructErrorMessageBody(const std::queue<MessageFormatElement>& messageFormat);

            /**
             * Provided both a header format and message format to follow, construct a 'peek' at the message that a
             * custom adapter failed to process correctly.
             * @param headerFormat  - Format for the message header.
             * @param messageFormat - Format for the message.
             */
            void ConstructErrorMessagePeek(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat);

            // Adapter data
            std::vector<Adapter*> _customAdapters;              // User-created custom adapters.
            StandardOutputAdapter* _cout;                       // Adapter for standard output (stdout)
            StandardOutputAdapter* _cerr;                       // Adapter for standard error (stderr)

            ProcessingAdapter* _errorMessageProcessing;         // Adapter for exception/error message processing.
            std::queue<std::string> _formattedErrorMessages;    // Storage for exception/error message - used during processing.

            // Message storage
            struct LogMessageStorage {
                /**
                 * Gather all data from the log message into one storage location while message is in queue to be processed.
                 * @param records         - Log message records.
                 * @param messageSeverity - Message severity.
                 * @param systemName      - Name of the system the message was logged through.
                 */
                LogMessageStorage(std::vector<LogMessage::LogRecord>& records, const LogMessageSeverity& messageSeverity, std::string systemName);

                /**
                 * Clears all stored data for this message.
                 */
                ~LogMessageStorage();

                std::vector<LogMessage::LogRecord> _records; // Log message record data.
                LogMessageSeverity _messageSeverity;         // Message severity.
                std::string _loggedSystemName;               // Name of the system the message was logged through.
            };

            // Threading data
            // While one buffer is being emptied, it should NOT be printed to.
            std::queue<LogMessageStorage> _printingBuffer1; // Buffer for holding message data stores.
            std::queue<LogMessageStorage> _printingBuffer2; // Buffer for holding message data stores.

            std::mutex _bufferMutex;                                             // Mutex to use while synchronizing threaded functions.
            std::chrono::milliseconds _asynchronousInterval;                     // Interval at which thread switches buffers and processes messages.
            std::atomic<bool> _distributeMessages;                               // Boolean flag to control the terminating condition for the threaded function.
            std::atomic<std::queue<LogMessageStorage>*> _printingBufferLocation; // Abstracting the location for functions to print data to.
            void* _currentMessageAddress;                                        // References the current packet of data being processed to verify passed pointers.
            void* _errorMessageAddress;                                          // In the case of an exception, references an additional packet of error data being processed to verify passed pointers.
            std::thread _distributingThread;                                     // Worker thread that processes error messages.

            std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime; // Timestamp for the initialization of the parent LoggingHub.
            std::atomic<bool> _reset;                                                        // Flag to prevent multiple resets of LoggingHubData
    };
    LoggingHub* LoggingHub::_loggingHub = nullptr;

    // Gather all data from the log message into one storage location while message is in queue to be processed.
    LoggingHub::LoggingHubData::LogMessageStorage::LogMessageStorage(std::vector<LogMessage::LogRecord>& records, const LogMessageSeverity& messageSeverity, std::string systemName) : _records(std::move(records)),
                                                                                                                                                                                       _messageSeverity(messageSeverity),
                                                                                                                                                                                       _loggedSystemName(std::move(systemName)) {
        // Nothing to do here.
    }

    // Clears all stored data for this message.
    LoggingHub::LoggingHubData::LogMessageStorage::~LogMessageStorage() {
        _records.clear();
    }

    // Initialize data for the LoggingHub to function properly. Starts an asynchronous worker thread working on the DistributeMessages function at a fixed interval.
    LoggingHub::LoggingHubData::LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp) : _cout(new StandardOutputAdapter("Standard Output", &std::cout)),
                                                                                                                                   _cerr(new StandardOutputAdapter("Standard Error", &std::cerr)),
                                                                                                                                   _errorMessageProcessing(new ProcessingAdapter("Adapter for processing error messages.")),
                                                                                                                                   _asynchronousInterval(std::chrono::milliseconds(200)),
                                                                                                                                   _distributeMessages(true),
                                                                                                                                   _printingBufferLocation(&_printingBuffer1),
                                                                                                                                   _currentMessageAddress(nullptr),
                                                                                                                                   _errorMessageAddress(nullptr),
                                                                                                                                   _initializationTime(initTimestamp),
                                                                                                                                   _reset(false)
                                                                                                                                   {
        _cerr->GetConfiguration().SetMessageSeverityCutoff(LogMessageSeverity::SEVERE);

        // Start worker thread last when all the other data is ready.
        _distributingThread = std::move(std::thread(&LoggingHub::LoggingHubData::DistributeMessages, this));
    }

    // Clean up the data related to the LoggingHub.
    LoggingHub::LoggingHubData::~LoggingHubData() {
        // Delete adapters.
        for (auto* adapter : _customAdapters) {
            delete adapter;
        }

        delete _cout;
        delete _cerr;
        delete _errorMessageProcessing;

        // Clear remaining exception/error formatted messages.
        while (!_formattedErrorMessages.empty()) {
            _formattedErrorMessages.pop();
        }
    }

    // Safely terminate the worker thread. Ensures all remaining messages will be outputted.
    void LoggingHub::LoggingHubData::TerminateWorkerThread() {
        // Only reset once.
        if (!_reset) {
            _reset = true;

            // Wait for both buffers to be completely empty.
            while (!_printingBuffer1.empty() || !_printingBuffer2.empty()) {
                std::this_thread::yield();
            }

            // Send 'shutdown' message to worker thread.
            _distributeMessages = false;

            // Pause the main thread to ensure worker thread has enough time to shutdown properly.
            _distributingThread.join();
        }
    }

    // Add a custom adapter to receive messages.
    void LoggingHub::LoggingHubData::AttachAdapter(Adapter* adapter) {
        _customAdapters.emplace_back(adapter);
    }

    // Remove a custom adapter from receiving messages. Calls destructor for the adapter.
    void LoggingHub::LoggingHubData::DetachAdapter(Adapter* adapter) {
        auto it = std::find(_customAdapters.begin(), _customAdapters.end(), adapter);
        if (it != _customAdapters.end()) {
            _customAdapters.erase(it);
            delete *it;
        }
    }

    // Get an adapter instance with a given name.
    Adapter* LoggingHub::LoggingHubData::GetAdapter(const std::string& name) {
        for (auto* adapter : _customAdapters) {
            if (adapter->GetName() == name) {
                return adapter;
            }
        }

        // Adapter doesn't exist, return nullptr.
        return nullptr;
    }

    // Verify message pointer. Used to safeguard against pointer modification and random data requests from sources that do not have access permissions to log message internals.
    bool LoggingHub::LoggingHubData::VerifyDataPointer(void* messageAddress) {
        return messageAddress == _currentMessageAddress || messageAddress == _errorMessageAddress;
    }

    // Transfer (move) log message data into internal data stores in the LoggingHub.
    void LoggingHub::LoggingHubData::Emplace(std::vector<LogMessage::LogRecord>& records, const LogMessageSeverity& messageSeverity, const std::string& systemName) {
        (*_printingBufferLocation).emplace(records, messageSeverity, systemName);
    }

    // Given a message pointer, safely get the log records of the underlying message.
    const std::vector<LogMessage::LogRecord>& LoggingHub::LoggingHubData::GetLogRecords(void* messageAddress) {
        if (VerifyDataPointer(messageAddress)) {
            return static_cast<LogMessageStorage*>(messageAddress)->_records;
        }

        // Invalid pointer provided, throw exception.
        throw std::invalid_argument("Invalid pointer provided to GetLogRecords.");
    }

    // Given a message pointer, safely get the message severity of the underlying message.
    const LogMessageSeverity& LoggingHub::LoggingHubData::GetMessageSeverity(void* messageAddress) {
        if (VerifyDataPointer(messageAddress)) {
            return static_cast<LogMessageStorage*>(messageAddress)->_messageSeverity;
        }

        // Invalid pointer provided, throw exception.
        throw std::invalid_argument("Invalid pointer provided to GetMessageSeverity.");
    }

    // Given a message pointer, safely get the name of the logging system that logged the message.
    const std::string& LoggingHub::LoggingHubData::GetThroughLoggingSystem(void* messageAddress) {
        if (VerifyDataPointer(messageAddress)) {
            return static_cast<LogMessageStorage*>(messageAddress)->_loggedSystemName;
        }

        // Invalid pointer provided, throw exception.
        throw std::invalid_argument("Invalid pointer provided to GetThroughLoggingSystem.");
    }

    // Get the initialization timestamp of the LoggingHub.
    const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::LoggingHubData::GetLoggingHubInitializationTimestamp() {
        return _initializationTime;
    }

    // Run by the worker thread to process and distribute message to adapters.
    void LoggingHub::LoggingHubData::DistributeMessages() {
        while (_distributeMessages) {
            // Only place that needs synchronization is switching the buffers, as everything else accesses the printing
            // buffers through an abstracted pointer.
            _bufferMutex.lock();
            std::queue<LogMessageStorage>& buffer = *_printingBufferLocation;
            SwitchBuffers();
            _bufferMutex.unlock();

            // Data will now be written to other buffer while this one is being printed from.
            while (!buffer.empty()) {
                LogMessageStorage& packet = buffer.front();
                _currentMessageAddress = static_cast<void*>(&packet);
                _errorMessageAddress = _currentMessageAddress;

                // Process message for standard output and standard error.
                _cout->ProcessMessage(_currentMessageAddress);
                _cout->OutputMessage();
                _cerr->ProcessMessage(_currentMessageAddress);
                _cerr->OutputMessage();

                // Process message for all custom adapters.
                for (auto* adapter : _customAdapters) {
                    try {
                        adapter->ProcessMessage(_currentMessageAddress);
                        adapter->OutputMessage();
                    }
                    // If the adapter failed to process safely and an exception was thrown from verifying the message pointer, it is caught here.
                    catch (const std::invalid_argument& exception) {

                        // Log a direct message to standard error about the exception.
                        _errorMessageProcessing->SetLogCount(_cerr->GetLogCount()); // Maintain previous log count.
                        ConstructStandardErrorMessage(exception, adapter->GetName().c_str());
                        _cerr->OutputErrorMessage(std::move(_formattedErrorMessages));

                        // Log a direct message to the adapter that failed to process the message safely.
                        _errorMessageProcessing->SetLogCount(adapter->GetLogCount()); // Maintain previous log count.
                        ConstructAdapterErrorMessage(exception, adapter->GetName().c_str());
                        adapter->OutputErrorMessage(std::move(_formattedErrorMessages));

                        // Delete faulty adapter from the custom adapters list.
                        DetachAdapter(adapter);
                    }
                }
                buffer.pop();
            }
            std::this_thread::sleep_for(_asynchronousInterval);
        }
    }

    // Switch the 'printed-to' buffer to be able to safely distribute the other buffer of stored message data.
    void LoggingHub::LoggingHubData::SwitchBuffers() {
        if (_printingBufferLocation == &(_printingBuffer1)) {
            _printingBufferLocation = &_printingBuffer2;
        }
        else {
            _printingBufferLocation = &_printingBuffer1;
        }
    }

    // Constructs the components of a message that is sent to the Standard Error adapter in the occurrence of an exception during message
    // processing by custom adapters.
    void LoggingHub::LoggingHubData::ConstructStandardErrorMessage(const std::invalid_argument& exception, const char* adapterName) {
        auto* errorMessage = new LogMessage(LogMessageSeverity::SEVERE);

        // Construct storage for message to use throughout processing.
        auto* processedMessageStorage = new LogMessageStorage(errorMessage->GetLogRecords(), errorMessage->GetMessageSeverity(), "LoggingHub error processing.");
        _currentMessageAddress = static_cast<void*>(processedMessageStorage);

        // Message header will follow the following format:
        //---------------------------------------------------------------------------------
        // [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
        //      - EXCEPTION -
        //---------------------------------------------------------------------------------
        std::queue<HeaderFormatElement> headerFormat;

        // Formatting for section: ---------------------------------------------------------------------------------
        headerFormat.push(HeaderFormatElement::SEPARATOR);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
        headerFormat.push(HeaderFormatElement::LOGCOUNT);
        headerFormat.push(HeaderFormatElement::BAR);
        headerFormat.push(HeaderFormatElement::DATE);
        headerFormat.push(HeaderFormatElement::BAR);
        headerFormat.push(HeaderFormatElement::SEVERITY);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: - EXCEPTION -
        headerFormat.push(HeaderFormatElement::TAB);
        headerFormat.push(HeaderFormatElement::DASH);
        headerFormat.push(HeaderFormatElement::EXCEPTION);
        headerFormat.push(HeaderFormatElement::DASH);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: ---------------------------------------------------------------------------------
        headerFormat.push(HeaderFormatElement::SEPARATOR);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        ConstructErrorMessageHeader(headerFormat);
        while (!headerFormat.empty()) {
            headerFormat.pop();
        }

        errorMessage->Supply("An exception occurred during message processing in custom adapter: %s.", adapterName);
        errorMessage->Supply("Exception message: %s\n", exception.what());
        errorMessage->Supply("Message:");
        processedMessageStorage->_records = std::move(errorMessage->GetLogRecords());

        // Message body will follow the following format:
        //    An exception occurred during message processing in custom adapter: ADAPTER_NAME.
        //    Exception message: EXCEPTION_MESSAGE
        //
        //    Message:
        std::queue<MessageFormatElement> messageFormat;

        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::MESSAGE);
        messageFormat.push(MessageFormatElement::NEWLINE);

        ConstructErrorMessageBody(messageFormat);
        while (!messageFormat.empty()) {
            messageFormat.pop();
        }

        // Message peek will follow the following format:
        //            [000m 00s 0000ms] - MESSAGE
        //                : supplied from (FUNCTION, FILE:LINE_NUMBER)

        // Formatting for section: [000m 00s 0000ms] - MESSAGE
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TIMESTAMP);
        messageFormat.push(MessageFormatElement::DASH);
        messageFormat.push(MessageFormatElement::MESSAGE);
        messageFormat.push(MessageFormatElement::NEWLINE);

#ifdef DEBUG_MESSAGES
        // Formatting for section: : supplied from (FUNCTION, FILE:LINE_NUMBER)
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::DEBUGINFO);
        messageFormat.push(MessageFormatElement::NEWLINE);
#endif

        messageFormat.push(MessageFormatElement::NEWLINE);

        ConstructErrorMessagePeek(headerFormat, messageFormat);
        while (!messageFormat.empty()) {
            messageFormat.pop();
        }

        // Formatting for section: ...
        errorMessage->Supply("...");
        processedMessageStorage->_records = std::move(errorMessage->GetLogRecords());

        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::MESSAGE);
        messageFormat.push(MessageFormatElement::NEWLINE);

        ConstructErrorMessageBody(messageFormat);
        while (!messageFormat.empty()) {
            messageFormat.pop();
        }
    }

    // Constructs the components of a message that is sent to the custom adapter in the occurrence of an exception during message processing.
    void LoggingHub::LoggingHubData::ConstructAdapterErrorMessage(const std::invalid_argument& exception, const char* adapterName) {
        auto* errorMessage = new LogMessage(LogMessageSeverity::SEVERE);

        // Construct storage for message to use throughout processing.
        auto* processedMessageStorage = new LogMessageStorage(errorMessage->GetLogRecords(), errorMessage->GetMessageSeverity(), "LoggingHub error processing.");
        _currentMessageAddress = static_cast<void*>(processedMessageStorage);

        // Message header will follow the following format:
        //---------------------------------------------------------------------------------
        // [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
        //      - EXCEPTION -
        //---------------------------------------------------------------------------------
        std::queue<HeaderFormatElement> headerFormat;

        // Formatting for section: ---------------------------------------------------------------------------------
        headerFormat.push(HeaderFormatElement::SEPARATOR);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
        headerFormat.push(HeaderFormatElement::LOGCOUNT);
        headerFormat.push(HeaderFormatElement::BAR);
        headerFormat.push(HeaderFormatElement::DATE);
        headerFormat.push(HeaderFormatElement::BAR);
        headerFormat.push(HeaderFormatElement::SEVERITY);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: - EXCEPTION -
        headerFormat.push(HeaderFormatElement::TAB);
        headerFormat.push(HeaderFormatElement::DASH);
        headerFormat.push(HeaderFormatElement::EXCEPTION);
        headerFormat.push(HeaderFormatElement::DASH);
        headerFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: ---------------------------------------------------------------------------------
        headerFormat.push(HeaderFormatElement::SEPARATOR);
        headerFormat.push(HeaderFormatElement::NEWLINE);
        ConstructErrorMessageHeader(headerFormat);


        errorMessage->Supply("An exception occurred during message processing - see standard error output for details.");
        errorMessage->Supply("The custom adapter with name '%s' will be removed from the LoggingHub adapter list.\n", adapterName);
        processedMessageStorage->_records = std::move(errorMessage->GetLogRecords());

        // Message body will follow the following format:
        //    An exception occurred during message processing - see standard error output for details.
        //    The custom adapter with name 'myAdapter' will be removed from the LoggingHub adapter list.
        std::queue<MessageFormatElement> messageFormat;
        messageFormat.push(MessageFormatElement::TAB);
        messageFormat.push(MessageFormatElement::MESSAGE);
        messageFormat.push(MessageFormatElement::NEWLINE);

        ConstructErrorMessageBody(messageFormat);
    }

    // Process the underlying message (provided by messageAddress) following the desired header and message formats. Stores the resulting processed error messages in the errorProcessingAdapter's formatted message store.
    void LoggingHub::LoggingHubData::ProcessMessageWithFormat(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat, void* messageAddress) {
        _errorMessageProcessing->GetConfiguration().PushHeaderFormat(headerFormat);
        _errorMessageProcessing->GetConfiguration().PushMessageFormat(messageFormat);
        _errorMessageProcessing->ProcessMessage(messageAddress);
        _errorMessageProcessing->GetConfiguration().PopHeaderFormat();
        _errorMessageProcessing->GetConfiguration().PopMessageFormat();
    }

    // Provided a format to follow, construct the header of an error message. This function can be used multiple times in the same error message.
    void LoggingHub::LoggingHubData::ConstructErrorMessageHeader(const std::queue<HeaderFormatElement>& headerFormat) {
        // Empty message format to skip message processing entirely.
        std::queue<MessageFormatElement> messageFormat;
        ProcessMessageWithFormat(headerFormat, messageFormat, _currentMessageAddress);

        // Push all messages to the output.
        std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
        while (!processedMessages.empty()) {
            _formattedErrorMessages.emplace(std::move(processedMessages.front()));
            processedMessages.pop();
        }

        // Clear remaining processing or extraneous messages.
        static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
        _errorMessageProcessing->ClearMessages();
    }

    // Provided a format to follow, construct a body component of an error message. This function can be used multiple times in the same error message.
    void LoggingHub::LoggingHubData::ConstructErrorMessageBody(const std::queue<MessageFormatElement>& messageFormat) {
        // Empty header format to skip header message processing entirely.
        std::queue<HeaderFormatElement> headerFormat;
        ProcessMessageWithFormat(headerFormat, messageFormat, _currentMessageAddress);

        // Push all messages to the output.
        std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
        while (!processedMessages.empty()) {
            _formattedErrorMessages.emplace(std::move(processedMessages.front()));
            processedMessages.pop();
        }

        // Clear remaining processing or extraneous messages.
        static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
        _errorMessageProcessing->ClearMessages();
    }

    // Provided both a header format and message format to follow, construct a 'peek' at the message that a custom adapter failed to process correctly.
    void LoggingHub::LoggingHubData::ConstructErrorMessagePeek(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat) {
        ProcessMessageWithFormat(headerFormat, messageFormat, _errorMessageAddress);

        // Formatting message peek: keep the first 3 lines (if it's a longer message) or until a newline is reached, whichever happens first.
        std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
        for (int i = 0; i < 3; ++i) {
            std::string& processedMessage = processedMessages.front();
            if (processedMessage == "\n") {
                break;
            }

            _formattedErrorMessages.emplace(std::move(processedMessage));
            processedMessages.pop();
        }

        // Clear remaining processing or extraneous messages.
        static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
        _errorMessageProcessing->ClearMessages();
    }


    //------------------------------------------------------------------------------------------------------------------
    // LOGGING HUB
    //------------------------------------------------------------------------------------------------------------------
    // Get a singleton LoggingHub instance.
    LoggingHub& Logger::LoggingHub::GetInstance() {
        if (!_loggingHub) {
            // Construct singleton
            _loggingHub = new LoggingHub();
        }

        return *_loggingHub;
    }

    // Direct call to initialize the necessary data for the LoggingHub to function properly.
    void LoggingHub::Initialize() {
        // Register destructor to be called on exit.
        int registrationValue = atexit(Reset);
        ASSERT(registrationValue == 0, "Registration of LogginHub Reset/Destructor function failed with code: %i", registrationValue);

        // Construct instance.
        GetInstance();
    }

    // Attach a custom adapter to process and receive messages through the LoggingHub.
    void Logger::LoggingHub::AttachCustomAdapter(Adapter* adapter) {
        _data->AttachAdapter(adapter);
    }

    // Retrieve a custom adapter with a provided name.
    Adapter* LoggingHub::GetCustomAdapter(const std::string& name) {
        return _data->GetAdapter(name);
    }

    // Used to verify the message pointer (provided to adapters' ProcessMessage function) before allowing access to internal message data.
    bool LoggingHub::VerifyDataPointer(void* messageAddress) {
        return _data->VerifyDataPointer(messageAddress);
    }

    // Given a message pointer, safely get the log records of the underlying message.
    const std::vector<LogMessage::LogRecord>& LoggingHub::GetLogRecords(void* messageAddress) {
        return _data->GetLogRecords(messageAddress);
    }

    // Given a message pointer, safely get the message severity of the underlying message.
    const LogMessageSeverity& LoggingHub::GetMessageSeverity(void* messageAddress) {
        return _data->GetMessageSeverity(messageAddress);
    }

    // Given a message pointer, safely get the name of the logging system that logged the message.
    const std::string& LoggingHub::GetThroughLoggingSystem(void* messageAddress) {
        return _data->GetThroughLoggingSystem(messageAddress);
    }

    // Get the initialization timestamp of the LoggingHub.
    const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::GetLoggingInitializationTime() {
        return _data->GetLoggingHubInitializationTimestamp();
    }

    // Private (singleton) constructor for an instance of the LoggingHub.
    Logger::LoggingHub::LoggingHub() : _data(std::move(std::make_unique<LoggingHubData>(std::chrono::high_resolution_clock::now()))) {
        // Nothing to do here.
    }

    // Destructor for the LoggingHub.
    Logger::LoggingHub::~LoggingHub() {
        Reset();
        delete _loggingHub;
        _loggingHub = nullptr;
    }

    // Direct call to shutdown the LoggingHub instance and flush all remaining messages.
    void LoggingHub::Reset() {
        _loggingHub->_data->TerminateWorkerThread();
        _loggingHub->_data.reset();
    }

    // Send a message through to the LoggingHub. Function only available to logging systems.
    void Logger::LoggingHub::SendMessage(LogMessage* message, const std::string& loggingSystemName) {
        _data->Emplace(message->GetLogRecords(), message->GetMessageSeverity(), loggingSystemName);
    }
}
