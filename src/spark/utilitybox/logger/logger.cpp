
#include <spark/core/rename.h>
#include <spark/utilitybox/logger/logger.h>                                // LoggingHub
#include <spark/utilitybox/logger/invalid_format_character_exception.h>    // InvalidFormatCharacterException
#include <spark/utilitybox/logger/adapter/adapter_config.h>                // AdapterConfiguration
#include <spark/utilitybox/logger/adapter/types/standard_output_adapter.h> // StandardOutputAdapter

namespace Spark::UtilityBox::Logger {
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
             * @param adapter - IAdapter instance to add.
             */
            void AttachAdapter(IAdapter* adapter);

            /**
             * Remove a custom adapter from receiving messages. Calls destructor for the adapter.
             * @param adapter - IAdapter instance to remove.
             */
            void DetachAdapter(IAdapter* adapter);

            /**
             * Get an adapter instance with a given name.
             * @param name - Name of the desired adapter.
             * @return On success: returns a pointer to the adapter.
             *         On failure: returns a null pointer.
             */
            IAdapter* GetAdapter(const char* name);

            /**
             * Transfer (move) log message data into internal data stores in the LoggingHub.
             * @param records         - Reference to the internal message storage for log records of the message - data will be moved entirely from this message.
             * @param messageSeverity - Severity of the passed message.
             * @param systemName      - System name of the logging system that logged this message.
             */
            void Emplace(LogRecord logRecord);

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

            NODISCARD std::vector<std::string> ProcessMessage(IAdapter* adapter, LogRecord* logRecord);
            NODISCARD std::vector<std::string> FormatMessage(const std::string &message, int characterStart, int characterEnd);
            NODISCARD std::string FormatCalendarInformation(const char *calendarFormatString);
            NODISCARD std::string FormatLogCount(int logCount);
            NODISCARD std::string FormatSeverity(const LogMessageSeverity &messageSeverity);
            NODISCARD std::string FormatTimestamp(const IAdapter *adapter, const Timing::TimeStamp &timeStamp);
            NODISCARD std::string FormatSeparator(int lineLength);

            // IAdapter data
            std::vector<IAdapter*> _adapterList;              // User-created custom adapters.
            StandardOutputAdapter* _cout;                       // IAdapter for standard output (stdout)
            StandardOutputAdapter* _cerr;                       // IAdapter for standard error (stderr)

            std::queue<std::string> _formattedErrorMessages;    // Storage for exception/error message - used during processing.

            // Threading data
            // While one buffer is being emptied, it should NOT be printed to.
            std::queue<LogRecord> _printingBuffer1; // Buffer for holding message data stores.
            std::queue<LogRecord> _printingBuffer2; // Buffer for holding message data stores.

            std::mutex _bufferMutex;                                             // Mutex to use while synchronizing threaded functions.
            std::chrono::milliseconds _asynchronousInterval;                     // Interval at which thread switches buffers and processes messages.
            std::atomic<bool> _distributeMessages;                               // Boolean flag to control the terminating condition for the threaded function.
            std::atomic<std::queue<LogRecord>*> _printingBufferLocation; // Abstracting the location for functions to print data to.
            std::thread _distributingThread;                                     // Worker thread that processes error messages.

            std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime; // Timestamp for the initialization of the parent LoggingHub.
            std::atomic<bool> _reset;                                                        // Flag to prevent multiple resets of LoggingHubData
    };

    LoggingHub* LoggingHub::_loggingHub = nullptr;

    // Initialize data for the LoggingHub to function properly. Starts an asynchronous worker thread working on the DistributeMessages function at a fixed interval.
    LoggingHub::LoggingHubData::LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp) : _cout(new StandardOutputAdapter("Standard Output", &std::cout)),
                                                                                                                                   _cerr(new StandardOutputAdapter("Standard Error", &std::cerr)),
                                                                                                                                   _asynchronousInterval(std::chrono::milliseconds(200)),
                                                                                                                                   _distributeMessages(true),
                                                                                                                                   _printingBufferLocation(&_printingBuffer1),
                                                                                                                                   _initializationTime(initTimestamp),
                                                                                                                                   _reset(false)
                                                                                                                                   {
        _cerr->GetConfiguration()->SetMessageSeverityCutoff(LogMessageSeverity::ERROR);

        AttachAdapter(_cout);
        AttachAdapter(_cerr);

        // Start worker thread last when all the other data is ready.
        _distributingThread = std::move(std::thread(&LoggingHub::LoggingHubData::DistributeMessages, this));
    }

    // Clean up the data related to the LoggingHub.
    LoggingHub::LoggingHubData::~LoggingHubData() {
        // Delete adapters.
        for (auto* adapter : _adapterList) {
            delete adapter;
        }

        delete _cout;
        delete _cerr;

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
    void LoggingHub::LoggingHubData::AttachAdapter(IAdapter* adapter) {
        try {
            adapter->Initialize();
        }
        catch (const std::runtime_error& exception) {
            LogRecord errorLogRecord;
            errorLogRecord.message = exception.what();
            errorLogRecord.messageSeverity = LogMessageSeverity::ERROR;
            errorLogRecord.loggingSystemName = "Logging Hub";

            static LogRecord* errorMessageAddress = &errorLogRecord;

            _cout->OutputMessage(ProcessMessage(_cout, errorMessageAddress));
            _cerr->OutputMessage(ProcessMessage(_cerr, errorMessageAddress));

            // Do not attach adapter.
            return;
        }

        _adapterList.emplace_back(adapter);
    }

    // Remove a custom adapter from receiving messages. Calls destructor for the adapter.
    void LoggingHub::LoggingHubData::DetachAdapter(IAdapter* adapter) {
        auto it = std::find(_adapterList.begin(), _adapterList.end(), adapter);
        if (it != _adapterList.end()) {
            _adapterList.erase(it);
            delete *it;
        }
    }

    // Get an adapter instance with a given name.
    IAdapter* LoggingHub::LoggingHubData::GetAdapter(const char* name) {
        for (auto* adapter : _adapterList) {
            if (!strcmp(adapter->GetConfiguration()->GetAdapterName(), name)) {
                return adapter;
            }
        }

        // IAdapter doesn't exist, return nullptr.
        return nullptr;
    }

    // Transfer (move) log message data into internal data stores in the LoggingHub.
    void LoggingHub::LoggingHubData::Emplace(LogRecord logRecord) {
        (*_printingBufferLocation).emplace(std::move(logRecord));
    }

    // Get the initialization timestamp of the LoggingHub.
    const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::LoggingHubData::GetLoggingHubInitializationTimestamp() {
        return _initializationTime;
    }

    // Run by the worker thread to process and distribute message to adapters.
    void LoggingHub::LoggingHubData::DistributeMessages() {
        while (_distributeMessages) {
            static LogRecord* currentMessageAddress = nullptr;

            // Only place that needs synchronization is switching the buffers, as everything else accesses the printing
            // buffers through an abstracted pointer.
            _bufferMutex.lock();
            std::queue<LogRecord>& buffer = *_printingBufferLocation;
            SwitchBuffers();
            _bufferMutex.unlock();

            // Data will now be written to other buffer while this one is being printed from.
            while (!buffer.empty()) {
                currentMessageAddress = &buffer.front();
                // Process message for standard output and standard error.
                static std::vector<std::string> formattedMessages;

                // Process message for all custom adapters.
                for (auto* adapter : _adapterList) {
                    try {
                        adapter->OutputMessage(std::move(ProcessMessage(adapter, currentMessageAddress)));
                    }
                    // If the adapter's formatting string contains an invalid character, an exception is thrown and caught here.
                    catch (const InvalidFormatCharacterException& exceptionData) {
                        LogRecord errorLogRecord;
                        errorLogRecord.message = exceptionData.what();
                        errorLogRecord.messageSeverity = LogMessageSeverity::ERROR;
                        errorLogRecord.loggingSystemName = "Logging Hub";

                        static LogRecord* errorMessageAddress = &errorLogRecord;

                        _cout->OutputMessage(std::move(ProcessMessage(_cout, errorMessageAddress)));
                        _cerr->OutputMessage(std::move(ProcessMessage(_cerr, errorMessageAddress)));

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

    std::vector<std::string> LoggingHub::LoggingHubData::ProcessMessage(IAdapter *adapter, LogRecord *logRecord) {
        static std::vector<std::string> processedMessages;

        if (logRecord->messageSeverity >= adapter->GetConfiguration()->GetMessageSeverityCutoff()) {
            static std::stringstream format;
            const char* adapterFormattingString = adapter->GetConfiguration()->GetFormattingString();

            int currentCharacterCount = 0;
            for (const char* traversalIndex = adapterFormattingString; *traversalIndex != '\0'; ++traversalIndex) {
                // Get to the character after the next '%'.
                while (*traversalIndex != '%') {
                    // Reached the end of the processing string.
                    if (*traversalIndex == '\0') {
                        return std::move(processedMessages);
                    }

                    // Emplace finished sentence and clear formatting stringstream.
                    if (*traversalIndex == '\n') {
                        format << std::endl;
                        processedMessages.emplace_back(format.str());
                        format.str(std::string(""));
                        currentCharacterCount = 0;
                    }
                    else if (*traversalIndex == '\t') {
                        format << *traversalIndex;
                        currentCharacterCount += 4;
                    }
                    else {
                        format << *traversalIndex;
                        ++currentCharacterCount;
                    }

                    ++traversalIndex;
                }
                ++traversalIndex;

                std::string appendedElement;

                switch (*traversalIndex) {
                    // Timestamp
                    case 't':
                        appendedElement = FormatTimestamp(adapter, logRecord->timeStamp);
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Log count
                    case 'c':
                        appendedElement = FormatLogCount(adapter->GetLogCount());
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Calendar date
                    case 'd':
                        appendedElement = FormatCalendarInformation(adapter->GetConfiguration()->GetCalendarFormatString());
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Message severity.
                    case 's':
                        appendedElement = FormatSeverity(logRecord->messageSeverity);
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Logged system name.
                    case 'n':
                        appendedElement = logRecord->loggingSystemName;
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Separating line.
                    case 'x':
                        appendedElement = FormatSeparator(adapter->GetConfiguration()->GetMessageWrapLimit());
                        format << appendedElement;
                        currentCharacterCount += appendedElement.length();
                        break;
                        // Message
                    case 'm':
                        for (auto& string : FormatMessage(logRecord->message, currentCharacterCount, adapter->GetConfiguration()->GetMessageWrapLimit())) {
                            format << string;
                        }
                        break;
                        // Unknown character.
                    default:
                        throw InvalidFormatCharacterException("Unknown formatting character: '%c' encountered in adapter formatting string. Custom adapter with name: '%s' will be removed from the custom adapter list.", *traversalIndex, adapter->GetConfiguration()->GetAdapterName());
                }
            }
        }

        return std::move(processedMessages);
    }

    std::vector<std::string> LoggingHub::LoggingHubData::FormatMessage(const std::string &message, int characterStart, int characterEnd) {
        static std::vector<std::string> formattedStrings;
        static std::stringstream format;

        int availableMessageSpace = characterEnd - characterStart;

        // Message cannot fit on one line, tokenize.
        if (message.length() > (availableMessageSpace)) {
            unsigned length = 0;                // Current length of the built string.
            bool firstLine = true;              // First line needs no aligning spaces, subsequent lines do.
            std::vector<std::string> tokens;    // Token storage.
            std::stringstream parser(message);  // Used to parse the message into tokens.
            std::string storage;                // Current token.

            // Tokenize the string by spaces.
            while (getline(parser, storage, ' ')) {
                // Delimiter gets rid of the space, we must append 1 to correctly count the space as a character.
                if ((length + storage.length() + 1) > (availableMessageSpace)) {

                    // First line has preceding information, don't append any extra characters to the beginning.
                    // Additional lines will need to be aligned.
                    if (!firstLine) {
                        format << std::setw(characterStart) << std::setfill(' ') << "";
                    }

                    firstLine = false;

                    // Include spaces between words.
                    for (auto &token : tokens) {
                        format << token << ' ';
                    }

                    // Reset.
                    format << std::endl;
                    formattedStrings.emplace_back(format.str());
                    format.str(std::string());
                    tokens.clear();
                    length = 0;
                }

                // String hasn't reached limit yet, append token to storage (bin string).
                length += storage.length() + 1;
                tokens.emplace_back(std::move(storage));
            }

            // Add all remaining tokens after the whole string has been parsed.
            if (!tokens.empty()) {
                format << std::setw(characterStart) << std::setfill(' ') << "";

                for (auto &token : tokens) {
                    format << token << ' ';
                }

                formattedStrings.emplace_back(format.str());
                format.str(std::string());
            }
        }

            // Message can fit on one line, no need to tokenize.
        else {
            formattedStrings.emplace_back(message);
        }

        return std::move(formattedStrings);
    }

    std::string LoggingHub::LoggingHubData::FormatCalendarInformation(const char *calendarFormatString) {
        static std::time_t _calendarTime = std::time(nullptr);  // Gets filled with the current time when formatting calendar buffer.
        static unsigned _calendarBufferSize = 64u;                    // Size of the calendar buffer.
        static char* _calendarBuffer = new char[_calendarBufferSize]; // Calendar buffer.

        while(!std::strftime(_calendarBuffer, _calendarBufferSize, calendarFormatString, std::localtime(&_calendarTime))) {
            _calendarBufferSize *= 2;
            delete[] _calendarBuffer;
            _calendarBuffer = new char[_calendarBufferSize];
        }

        // Reset to get the most updated calendar time.
        _calendarTime = std::time(nullptr);

        return _calendarBuffer;
    }

    std::string LoggingHub::LoggingHubData::FormatLogCount(int logCount) {
        static std::stringstream format;

        format << std::setfill('0') << std::setw(4);
        format << logCount / 1000;

        format << ' ';

        format << std::setfill('0') << std::setw(4);
        format << logCount % 1000;

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string LoggingHub::LoggingHubData::FormatSeverity(const LogMessageSeverity &messageSeverity) {
        static std::stringstream format;

        switch (messageSeverity) {
            case LogMessageSeverity::DEBUG:
                format << "  DEBUG  ";
                break;
            case LogMessageSeverity::WARNING:
                format << " WARNING ";
                break;
            case LogMessageSeverity::ERROR:
                format << "  ERROR  ";
                break;
        }

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string LoggingHub::LoggingHubData::FormatTimestamp(const IAdapter *adapter, const Timing::TimeStamp &timeStamp) {
        static std::stringstream format;

        for (const char* traversalIndex = adapter->GetConfiguration()->GetTimestampFormattingString(); *traversalIndex != '\0'; ++traversalIndex) {
            // Get to the character after the next '%'.
            while (*traversalIndex != '%') {
                // Reached the end of the string.
                if (*traversalIndex == '\0') {
                    std::string formattedMessage = format.str();
                    format.str(std::string("")); // Clear format.
                    return std::move(formattedMessage);
                }

                format << *traversalIndex++;
            }
            ++traversalIndex;

            switch (*traversalIndex) {
                // Append minutes into stream.
                case 'm':
                    format << std::setfill('0') << std::setw(3);
                    format << timeStamp.GetMinutes();
                    break;
                    // Append seconds into stream.
                case 's':
                    format << std::setfill('0') << std::setw(2);
                    format << timeStamp.GetSeconds();
                    break;
                    // Append milliseconds into stream.
                case 'l':
                    format << std::setfill('0') << std::setw(4);
                    format << timeStamp.GetMillis();
                    break;
                default:
                    throw InvalidFormatCharacterException("Unknown timestamp formatting character: '%c' encountered in adapter timestamp formatting string. Custom adapter with name: '%s' will be removed from the custom adapter list.", *traversalIndex, adapter->GetConfiguration()->GetAdapterName());
            }
        }

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }

    std::string LoggingHub::LoggingHubData::FormatSeparator(int lineLength) {
        static std::stringstream format;
        format << std::setw(lineLength) << std::setfill('-') << "";

        std::string formattedMessage = format.str();
        format.str(std::string("")); // Clear format.
        return std::move(formattedMessage);
    }


    //------------------------------------------------------------------------------------------------------------------
    // LOGGING HUB
    //------------------------------------------------------------------------------------------------------------------
    // Get a singleton LoggingHub instance.
    LoggingHub* Logger::LoggingHub::GetInstance() {
        if (!_loggingHub) {
            _loggingHub = new LoggingHub();
        }
        return _loggingHub;
    }

    // Attach a custom adapter to process and receive messages through the LoggingHub.
    void Logger::LoggingHub::AttachCustomAdapter(IAdapter* adapter) {
        _data->AttachAdapter(adapter);
    }

    IAdapter* Logger::LoggingHub::GetCustomAdapter(const char* adapterName) {
        return _data->GetAdapter(adapterName);
    }

    void LoggingHub::DetachCustomAdapter(IAdapter *adapter) {
        _data->DetachAdapter(adapter);
    }

    // Get the initialization timestamp of the LoggingHub.
    const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::GetInitializationTime() {
        return _data->GetLoggingHubInitializationTimestamp();
    }

    // Private (singleton) constructor for an instance of the LoggingHub.
    Logger::LoggingHub::LoggingHub() : _data(new LoggingHubData(std::chrono::high_resolution_clock::now())) {
        // Nothing to do here.
    }

    // Destructor for the LoggingHub.
    Logger::LoggingHub::~LoggingHub() {
        _loggingHub->_data->TerminateWorkerThread();
        delete _loggingHub->_data;

        delete _loggingHub;
        _loggingHub = nullptr;
    }


    // Send a message through to the LoggingHub. Function only available to logging systems.
    void Logger::LoggingHub::SendMessage(LogRecord logRecord) {
        _data->Emplace(std::move(logRecord));
    }
}
