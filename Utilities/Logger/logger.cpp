#include "logger.h"
#include "../assert_dev.h"
#include "../exception.h"
#include <mutex>
#include <ostream>
#include <utility>

namespace UtilityBox {
    namespace Logger {
        class StandardOutputAdapter final : public Adapter {
            public:
                explicit StandardOutputAdapter(std::string&& name, std::ostream* stream);
                ~StandardOutputAdapter() override;
                void OutputMessage() override;

            private:
                std::ostream* _stream;
        };

        StandardOutputAdapter::StandardOutputAdapter(std::string&& name, std::ostream* stream) : Adapter(std::move(name)), _stream(stream) {
        }

        void StandardOutputAdapter::OutputMessage() {
            while (!_formattedMessages.empty()) {
                const std::string& message = _formattedMessages.front();
                (*_stream) << message;
                _formattedMessages.pop();
            }

            // add gaps between consecutive messages
            (*_stream) << "\n";
            (*_stream).flush();

            ClearMessages();
            ++_logCount;
        }

        StandardOutputAdapter::~StandardOutputAdapter() {
            ClearMessages();
        }

        class ProcessingAdapter : public Adapter {
            public:
                explicit ProcessingAdapter(std::string&& name);
                void OutputMessage() override;
                std::queue<std::string>& GetFormattedMessages();
        };

        ProcessingAdapter::ProcessingAdapter(std::string &&name) : Adapter(std::move(name)) {
            // nothing
        }

        void ProcessingAdapter::OutputMessage() {
            // nothing
        }

        std::queue<std::string>& ProcessingAdapter::GetFormattedMessages() {
            return _formattedMessages;
        }

        LoggingHub* LoggingHub::_loggingHub = nullptr;

        class LoggingHub::LoggingHubData {
            public:
                void TerminateWorkerThread();
                void AttachAdapter(Adapter *adapter);
                Adapter* GetAdapter(const std::string& name);
                void Emplace(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity, const std::string& systemName);

                bool VerifyDataPointer(void* messageAddress);
                const std::vector<LogMessage::LogRecord>& GetLogRecords(void *messageAddress);
                const LogMessageSeverity& GetMessageSeverity(void *messageAddress);
                const std::string& GetThroughLoggingSystem(void *messageAddress);
                const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingHubInitializationTimestamp();

                explicit LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp);
                ~LoggingHubData();

            private:
                StandardOutputAdapter* stdcout;
                StandardOutputAdapter* stdcerr;

                // error message processing
                ProcessingAdapter* _errorMessageProcessing;
                std::queue<std::string> _formattedErrorMessages;
                void ConstructStandardErrorMessage(const Exceptions::Exception& exception, const char* adapterName);
                void ConstructAdapterErrorMessage(const Exceptions::Exception& exception);
                void ConstructErrorMessageHeader(std::queue<HeaderFormatElement>& headerFormat);
                void ConstructErrorMessageBody(std::queue<MessageFormatElement>& messageFormat);
                void ConstructErrorMessagePeek(std::queue<HeaderFormatElement>& headerFormat, std::queue<MessageFormatElement>& messageFormat);
                void ProcessMessageWithFormat(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat, void* messageAddress);

                void DistributeMessages();
                void SwitchBuffers();

                // Adapters
                std::vector<Adapter*> _customAdapters;

                // Message storage
                struct LogMessageStorage {
                    LogMessageStorage(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity, std::string systemName);
                    ~LogMessageStorage();
                    std::vector<LogMessage::LogRecord> _records;
                    LogMessageSeverity _messageSeverity;
                    std::string _loggedSystemName;
                };

                std::queue<LogMessageStorage> _printingBuffer1;
                std::queue<LogMessageStorage> _printingBuffer2;

                // threading
                std::mutex _bufferMutex;
                std::chrono::milliseconds _asynchronousInterval;
                std::atomic<bool> _distributeMessages;
                std::atomic<std::queue<LogMessageStorage>*> _printingBufferLocation;
                void* _currentMessageAddress;
                void* _errorMessageAddress;
                std::thread _distributingThread;
                std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime;
        };

        LoggingHub::LoggingHubData::LogMessageStorage::LogMessageStorage(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity, std::string systemName) : _records(std::move(records)), _messageSeverity(messageSeverity), _loggedSystemName(std::move(systemName)) {
        }

        LoggingHub::LoggingHubData::LogMessageStorage::~LogMessageStorage() {
            _records.clear();
        }

        // starts an asynchronous worker thread working on the Distribute function once per asynchronousInterval. Uses _distributeMessages as a toggle.
        LoggingHub::LoggingHubData::LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp) : _initializationTime(initTimestamp), _asynchronousInterval(std::chrono::milliseconds(100)), _distributeMessages(false), _printingBufferLocation(&_printingBuffer1) {
            // standard output (cout) and error (cerr) adapters
            stdcout = new StandardOutputAdapter("Standard Output", &std::cout);
            stdcerr = new StandardOutputAdapter("Standard Error", &std::cerr);
            stdcerr->GetConfiguration().SetMessageSeverityCutoff(LogMessageSeverity::SEVERE);

            // yield thread performance until logging hub is ready
            _errorMessageProcessing = new ProcessingAdapter("Adapter for processing error messages.");
            _distributeMessages = true;
            _distributingThread = std::thread(&LoggingHub::LoggingHubData::DistributeMessages, this);
            _currentMessageAddress = nullptr;
            _errorMessageAddress = nullptr;
        }

        void LoggingHub::LoggingHubData::DistributeMessages() {
            while (_distributeMessages) {
                _bufferMutex.lock();
                std::queue<LogMessageStorage>& buffer = *_printingBufferLocation;
                SwitchBuffers();
                _bufferMutex.unlock();

                // data will now be written to other buffer while this one is being printed from
                while (!buffer.empty()) {
                    LogMessageStorage& packet = buffer.front();
                    _currentMessageAddress = static_cast<void*>(&packet);
                    _errorMessageAddress = _currentMessageAddress;
                    for (auto* adapter : _customAdapters) {
                        try {
                            const int& logCount = adapter->GetLogCount();
                            adapter->ProcessMessage(_currentMessageAddress);
                            adapter->OutputMessage();
                            if (logCount + 1 != adapter->GetLogCount()) {
                                adapter->SetLogCount(logCount + 1);
                            }
                        }
                        catch (const Exceptions::Exception& exception) {
                            // send a message to std err about the exception
                            _errorMessageProcessing->SetLogCount(stdcerr->GetLogCount());
                            ConstructStandardErrorMessage(exception, adapter->GetName().c_str());
                            stdcerr->OutputErrorMessage(_formattedErrorMessages);

                            // log a message to the adapter about the exception
                            const int& logCount = adapter->GetLogCount();
                            _errorMessageProcessing->SetLogCount(logCount);
                            ConstructAdapterErrorMessage(exception);
                            adapter->OutputErrorMessage(_formattedErrorMessages);
                            if (logCount + 1 != adapter->GetLogCount()) {
                                adapter->SetLogCount(logCount + 1);
                            }
                        }
                    }
                    buffer.pop();
                }

                std::this_thread::sleep_for(_asynchronousInterval);
            }
        }

        void LoggingHub::LoggingHubData::ConstructErrorMessageHeader(std::queue<HeaderFormatElement>& headerFormat) {
            std::queue<MessageFormatElement> messageFormat;
            ProcessMessageWithFormat(headerFormat, messageFormat, _currentMessageAddress);
            std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
            while (!processedMessages.empty()) {
                _formattedErrorMessages.emplace(std::move(processedMessages.front()));
                processedMessages.pop();
            }

            static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
            _errorMessageProcessing->ClearMessages();

            // clear formats
            while (!headerFormat.empty()) {
                headerFormat.pop();
            }
        }

        void LoggingHub::LoggingHubData::ConstructErrorMessageBody(std::queue<MessageFormatElement>& messageFormat) {
            std::queue<HeaderFormatElement> headerFormat;
            ProcessMessageWithFormat(headerFormat, messageFormat, _currentMessageAddress);
            std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
            while (!processedMessages.empty()) {
                _formattedErrorMessages.emplace(std::move(processedMessages.front()));
                processedMessages.pop();
            }

            static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
            _errorMessageProcessing->ClearMessages();

            // clear formats
            while (!messageFormat.empty()) {
                messageFormat.pop();
            }
        }

        void LoggingHub::LoggingHubData::ConstructErrorMessagePeek(std::queue<HeaderFormatElement>& headerFormat, std::queue<MessageFormatElement>& messageFormat) {
            ProcessMessageWithFormat(headerFormat, messageFormat, _errorMessageAddress);
            std::queue<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
            for (int i = 0; i < 3; ++i) {
                std::string& processedMessage = processedMessages.front();
                if (processedMessage == "\n") {
                    break;
                }

                _formattedErrorMessages.emplace(std::move(processedMessage));
                processedMessages.pop();
            }

            static_cast<LogMessageStorage*>(_currentMessageAddress)->_records.clear();
            _errorMessageProcessing->ClearMessages();

            // clear formats
            while (!headerFormat.empty()) {
                headerFormat.pop();
            }
            while (!messageFormat.empty()) {
                messageFormat.pop();
            }
        }

        void LoggingHub::LoggingHubData::ProcessMessageWithFormat(const std::queue<HeaderFormatElement>& headerFormat, const std::queue<MessageFormatElement>& messageFormat, void* messageAddress) {
            _errorMessageProcessing->GetConfiguration().AddCustomHeaderFormatStructure(headerFormat);
            _errorMessageProcessing->GetConfiguration().AddCustomMessageFormatStructure(messageFormat);
            _errorMessageProcessing->ProcessMessage(messageAddress);
            _errorMessageProcessing->GetConfiguration().RevertHeaderFormat();
            _errorMessageProcessing->GetConfiguration().RevertMessageFormat();
        }

        void LoggingHub::LoggingHubData::SwitchBuffers() {
            if (_printingBufferLocation == &(_printingBuffer1)) {
                _printingBufferLocation = &_printingBuffer2;
            }
            else {
                _printingBufferLocation = &_printingBuffer1;
            }
        }

        LoggingHub::LoggingHubData::~LoggingHubData() {
//            for (auto* adapter : _adapters) {
//                delete adapter;
//            }
//            _adapters.clear();
        }

        void LoggingHub::LoggingHubData::AttachAdapter(Adapter *adapter) {
            _customAdapters.emplace_back(adapter);
        }

        void LoggingHub::LoggingHubData::Emplace(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity, const std::string& systemName) {
            (*_printingBufferLocation).emplace(std::move(records), messageSeverity, systemName);
        }

        const std::vector<LogMessage::LogRecord> &LoggingHub::LoggingHubData::GetLogRecords(void *messageAddress) {
            if (VerifyDataPointer(messageAddress)) {
                return static_cast<LogMessageStorage*>(messageAddress)->_records;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetLogRecords.");
        }

        const LogMessageSeverity &LoggingHub::LoggingHubData::GetMessageSeverity(void *messageAddress) {
            if (VerifyDataPointer(messageAddress)) {
                return static_cast<LogMessageStorage*>(messageAddress)->_messageSeverity;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageSeverity.");
        }

        const std::string& LoggingHub::LoggingHubData::GetThroughLoggingSystem(void *messageAddress) {
            if (VerifyDataPointer(messageAddress)) {
                return static_cast<LogMessageStorage*>(messageAddress)->_loggedSystemName;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetThroughLoggingSystem.");
        }

        const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::LoggingHubData::GetLoggingHubInitializationTimestamp() {
            return _initializationTime;
        }

        Adapter* LoggingHub::LoggingHubData::GetAdapter(const std::string &name) {
            for (auto* adapter : _customAdapters) {
                if (adapter->GetName() == name) {
                    return adapter;
                }
            }

            return nullptr;
        }

        void LoggingHub::LoggingHubData::TerminateWorkerThread() {
            // wait for both buffers to be completely empty
            while (!_printingBuffer1.empty() || !_printingBuffer2.empty()) {
                std::this_thread::yield();
            }

            // send 'shutdown' message to worker thread
            _distributeMessages = false;

            // pause the main thread to ensure worker thread has enough time to process 'shutdown' message
            _distributingThread.join();
        }

        bool LoggingHub::LoggingHubData::VerifyDataPointer(void *messageAddress) {
            return messageAddress == _currentMessageAddress || messageAddress == _errorMessageAddress;
        }

        void LoggingHub::LoggingHubData::ConstructStandardErrorMessage(const Exceptions::Exception& exception, const char* adapterName) {
            // construct message
            auto* errorMessage = new LogMessage(LogMessageSeverity::SEVERE);

            // construct storage for message to use throughout processing
            auto* processedMessageStorage = new LogMessageStorage(std::move(*errorMessage->GetLogMessages()), errorMessage->GetMessageSeverity(), "LoggingHub error processing.");
            _currentMessageAddress = static_cast<void*>(processedMessageStorage);
            std::queue<MessageFormatElement> messageFormat;

            // construct header for stdcerr error message
            std::queue<HeaderFormatElement> headerFormat = _errorMessageProcessing->GetConfiguration().GetDefaultHeaderFormat();
            ConstructErrorMessageHeader(headerFormat);

            errorMessage->Supply("An exception occurred during message processing in custom adapter: %s.", adapterName);
            errorMessage->Supply("Exception message: %s\n", exception.what());
            errorMessage->Supply("Message:");
            processedMessageStorage->_records = std::move(*errorMessage->GetLogMessages());

            // construct description for stdcerr error message
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            ConstructErrorMessageBody(messageFormat);

            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TIMESTAMP);
            messageFormat.push(MessageFormatElement::DASH);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);

#ifdef DEBUG_MESSAGES
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::DEBUGINFO);
            messageFormat.push(MessageFormatElement::NEWLINE);
#endif

            messageFormat.push(MessageFormatElement::NEWLINE);
            ConstructErrorMessagePeek(headerFormat, messageFormat);

            // construct ellipsis at the end
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::ELLIPSIS);
            messageFormat.push(MessageFormatElement::NEWLINE);
            ConstructErrorMessageBody(messageFormat);
        }

        void LoggingHub::LoggingHubData::ConstructAdapterErrorMessage(const Exceptions::Exception &exception) {
            // construct message
            auto* errorMessage = new LogMessage(LogMessageSeverity::SEVERE);

            // construct storage for message to use throughout processing
            auto* processedMessageStorage = new LogMessageStorage(std::move(*errorMessage->GetLogMessages()), errorMessage->GetMessageSeverity(), "LoggingHub error processing.");
            _currentMessageAddress = static_cast<void*>(processedMessageStorage);
            std::queue<MessageFormatElement> messageFormat;

            // construct header for stdcerr error message
            std::queue<HeaderFormatElement> headerFormat = _errorMessageProcessing->GetConfiguration().GetDefaultHeaderFormat();
            ConstructErrorMessageHeader(headerFormat);

            errorMessage->Supply("An exception occurred during message processing. See standard error output for details.");
            processedMessageStorage->_records = std::move(*errorMessage->GetLogMessages());

            // construct description for stdcerr error message
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            ConstructErrorMessageBody(messageFormat);
        }

        LoggingHub& Logger::LoggingHub::GetInstance() {
            if (!_loggingHub) {
                _loggingHub = new LoggingHub();
            }

            return *_loggingHub;
        }

        void LoggingHub::Initialize() {
            GetInstance();
        }

        void LoggingHub::Reset() {
            _loggingHub->_data->TerminateWorkerThread();
            _loggingHub->_data.reset();
        }

        Logger::LoggingHub::LoggingHub() : _data(std::move(std::make_unique<LoggingHubData>(std::chrono::high_resolution_clock::now()))) {
            // nothing
        }

        Logger::LoggingHub::~LoggingHub() {
            delete _loggingHub;
            _loggingHub = nullptr;
        }

        void Logger::LoggingHub::SendMessage(LogMessage &&message, const std::string& loggingSystemName) {
            _data->Emplace(std::move(*message.GetLogMessages()), message.GetMessageSeverity(), loggingSystemName);
        }

        void Logger::LoggingHub::AttachCustomAdapter(Adapter *adapter) {
            _data->AttachAdapter(adapter);
        }

        const std::vector<LogMessage::LogRecord>& LoggingHub::GetLogRecords(void *messageAddress) {
            return _data->GetLogRecords(messageAddress);
        }

        const LogMessageSeverity &LoggingHub::GetMessageSeverity(void *messageAddress) {
            return _data->GetMessageSeverity(messageAddress);
        }

        const std::string &LoggingHub::GetThroughLoggingSystem(void *messageAddress) {
            return _data->GetThroughLoggingSystem(messageAddress);
        }

        const std::chrono::time_point<std::chrono::high_resolution_clock> &LoggingHub::GetLoggingInitializationTime() {
            return _data->GetLoggingHubInitializationTimestamp();
        }

        Adapter* LoggingHub::GetCustomAdapter(const std::string &name) {
            return _data->GetAdapter(name);
        }

        bool LoggingHub::VerifyDataPointer(void *messageAddress) {
            return _data->VerifyDataPointer(messageAddress);
        }
    }
}
