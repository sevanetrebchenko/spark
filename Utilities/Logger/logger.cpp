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
            for (auto& message : _formattedMessages) {
                (*_stream) << message;
            }

            // add gaps between consecutive messages
            (*_stream) << "\n";
            _formattedMessages.clear();

            (*_stream).flush();
        }

        StandardOutputAdapter::~StandardOutputAdapter() {
            _formattedMessages.clear();
        }

        class ProcessingAdapter : public Adapter {
            public:
                explicit ProcessingAdapter(std::string&& name);
                void OutputMessage() override;
                std::vector<std::string>& GetFormattedMessages();
        };

        ProcessingAdapter::ProcessingAdapter(std::string &&name) : Adapter(std::move(name)) {
            // nothing
        }

        void ProcessingAdapter::OutputMessage() {
            _formattedMessages.clear();
        }

        std::vector<std::string>& ProcessingAdapter::GetFormattedMessages() {
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
                void FormatHeader(std::queue<std::string>& messagePool, LogMessage* errorMessage, StandardOutputAdapter* stdcerr);
                void FormatExceptionMessage(std::queue<std::string>& messagePool, LogMessage* errorMessage, StandardOutputAdapter* stdcerr, const Exceptions::Exception& exception, const char* adapterName);
                void FormatMessagePeek(std::queue<std::string>& messagePool, LogMessage* errorMessage, StandardOutputAdapter* stdcerr, void* errorMessageDataAddress);
                void DistributeMessages();
                void SwitchBuffers();

                // Adapters
                std::vector<Adapter*> _customAdapters;
                ProcessingAdapter* _errorMessageProcessing;

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
            auto* stdcout = new StandardOutputAdapter("Standard Output", &std::cout);
            auto* stdcerr = new StandardOutputAdapter("Standard Error", &std::cerr);
            stdcerr->GetConfiguration().SetMessageSeverityCutoff(LogMessageSeverity::SEVERE);
            AttachAdapter(stdcout);
            AttachAdapter(stdcerr);

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
                    _errorMessageAddress = static_cast<void*>(&packet);
                    for (auto* adapter : _customAdapters) {
                        try {
                            adapter->ProcessMessage(_currentMessageAddress);
                            adapter->OutputMessage();
                        }
                        catch (const Exceptions::Exception& exception) {
                            Adapter* error = GetAdapter("Standard Error");
                            ASSERT(ASSERT_LEVEL_FATAL, error != nullptr, "Failed to get Standard Error adapter.");
                            if (auto* stdcerr = dynamic_cast<StandardOutputAdapter*>(error)) {
                                // construct message
                                std::queue<std::string> messages;
                                auto* errorMessage = new LogMessage(LogMessageSeverity::SEVERE);
                                FormatHeader(messages, errorMessage, stdcerr);
                                _errorMessageProcessing->OutputMessage();
                                FormatExceptionMessage(messages, errorMessage, stdcerr, exception, adapter->GetName().c_str());
                                _errorMessageProcessing->OutputMessage();
                                FormatMessagePeek(messages, errorMessage, stdcerr, _errorMessageAddress);
                                _errorMessageProcessing->OutputMessage();

                                stdcerr->OutputErrorMessage(std::move(messages));
                            }
                        }
                    }
                    buffer.pop();
                }

                std::this_thread::sleep_for(_asynchronousInterval);
            }
        }

        void LoggingHub::LoggingHubData::FormatHeader(std::queue<std::string>& messagePool, LogMessage* errorMessage, StandardOutputAdapter *stdcerr) {
            auto* processedMessageStorage = new LogMessageStorage(std::move(*errorMessage->GetLogMessages()), errorMessage->GetMessageSeverity(), "LoggingHub error processing.");
            _currentMessageAddress = static_cast<void*>(processedMessageStorage);

            std::queue<HeaderFormatElement> headerFormat;
            headerFormat.push(HeaderFormatElement::SEPARATOR);
            headerFormat.push(HeaderFormatElement::NEWLINE);
            headerFormat.push(HeaderFormatElement::TERMINATOR);
            headerFormat.push(HeaderFormatElement::LOGCOUNT);
            headerFormat.push(HeaderFormatElement::BAR);
            headerFormat.push(HeaderFormatElement::DATE);
            headerFormat.push(HeaderFormatElement::BAR);
            headerFormat.push(HeaderFormatElement::SEVERITY);
            headerFormat.push(HeaderFormatElement::NEWLINE);
            headerFormat.push(HeaderFormatElement::SEPARATOR);
            headerFormat.push(HeaderFormatElement::NEWLINE);
            headerFormat.push(HeaderFormatElement::TERMINATOR);

            std::queue<MessageFormatElement> messageFormat;
            _errorMessageProcessing->GetConfiguration().AddCustomHeaderFormatStructure(headerFormat);
            _errorMessageProcessing->GetConfiguration().AddCustomMessageFormatStructure(messageFormat);
            _errorMessageProcessing->ProcessMessage(_currentMessageAddress);
            for (auto& message : _errorMessageProcessing->GetFormattedMessages()) {
                messagePool.emplace(std::move(message));
            }
            _errorMessageProcessing->GetConfiguration().RevertHeaderFormat();
            _errorMessageProcessing->GetConfiguration().RevertMessageFormat();
        }

        void LoggingHub::LoggingHubData::FormatExceptionMessage(std::queue<std::string>& messagePool, LogMessage *errorMessage, StandardOutputAdapter *stdcerr, const Exceptions::Exception& exception, const char* adapterName) {
            errorMessage->Supply("An exception occurred during message processing in custom adapter: %s.", adapterName);
            errorMessage->Supply("Exception message: %s\n", exception.what());

            auto* processedMessageStorage = new LogMessageStorage(std::move(*errorMessage->GetLogMessages()), errorMessage->GetMessageSeverity(), "");
            _currentMessageAddress = static_cast<void*>(processedMessageStorage);

            std::queue<HeaderFormatElement> headerFormat;

            std::queue<MessageFormatElement> messageFormat;
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::TERMINATOR);

            _errorMessageProcessing->GetConfiguration().AddCustomHeaderFormatStructure(headerFormat);
            _errorMessageProcessing->GetConfiguration().AddCustomMessageFormatStructure(messageFormat);
            _errorMessageProcessing->ProcessMessage(_currentMessageAddress);
            for (auto& message : _errorMessageProcessing->GetFormattedMessages()) {
                messagePool.emplace(std::move(message));
            }
            _errorMessageProcessing->GetConfiguration().RevertHeaderFormat();
            _errorMessageProcessing->GetConfiguration().RevertMessageFormat();
        }

        void LoggingHub::LoggingHubData::FormatMessagePeek(std::queue<std::string>& messagePool, LogMessage *errorMessage, StandardOutputAdapter *stdcerr, void* errorMessageDataAddress) {
            errorMessage->Supply("Message:");

            auto* processedMessageStorage = new LogMessageStorage(std::move(*errorMessage->GetLogMessages()), errorMessage->GetMessageSeverity(), "");
            _currentMessageAddress = static_cast<void*>(processedMessageStorage);

            std::queue<HeaderFormatElement> headerFormat;

            std::queue<MessageFormatElement> messageFormat;
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::TERMINATOR);

            _errorMessageProcessing->GetConfiguration().AddCustomHeaderFormatStructure(headerFormat);
            _errorMessageProcessing->GetConfiguration().AddCustomMessageFormatStructure(messageFormat);
            _errorMessageProcessing->ProcessMessage(_currentMessageAddress);
            for (auto& message : _errorMessageProcessing->GetFormattedMessages()) {
                messagePool.emplace(std::move(message));
            }
            _errorMessageProcessing->OutputMessage();
            _errorMessageProcessing->GetConfiguration().RevertHeaderFormat();
            _errorMessageProcessing->GetConfiguration().RevertMessageFormat();

            while (!messageFormat.empty()) {
                messageFormat.pop();
            }

            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TIMESTAMP);
            messageFormat.push(MessageFormatElement::DASH);
            messageFormat.push(MessageFormatElement::MESSAGE);
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::TERMINATOR);
#ifdef DEBUG_MESSAGES
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::TAB);
            messageFormat.push(MessageFormatElement::DEBUGINFO);
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::TERMINATOR);
#endif
            messageFormat.push(MessageFormatElement::NEWLINE);
            messageFormat.push(MessageFormatElement::TERMINATOR);

            _errorMessageProcessing->GetConfiguration().AddCustomHeaderFormatStructure(headerFormat);
            _errorMessageProcessing->GetConfiguration().AddCustomMessageFormatStructure(messageFormat);
            _errorMessageProcessing->ProcessMessage(errorMessageDataAddress);
            _errorMessageProcessing->GetConfiguration().RevertHeaderFormat();
            _errorMessageProcessing->GetConfiguration().RevertMessageFormat();

            std::vector<std::string>& processedMessages = _errorMessageProcessing->GetFormattedMessages();
            for (int i = 0; i < 12; ++i) {
                if (processedMessages.at(i) == "\n") {
                    break;
                }

                messagePool.emplace(std::move(processedMessages.at(i)));
            }
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
            if (messageAddress == _currentMessageAddress || messageAddress == _errorMessageAddress) {
                return static_cast<LogMessageStorage*>(messageAddress)->_records;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetLogRecords.");
        }

        const LogMessageSeverity &LoggingHub::LoggingHubData::GetMessageSeverity(void *messageAddress) {
            if (messageAddress == _currentMessageAddress || messageAddress == _errorMessageAddress) {
                return static_cast<LogMessageStorage*>(messageAddress)->_messageSeverity;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageSeverity.");
        }

        const std::string& LoggingHub::LoggingHubData::GetThroughLoggingSystem(void *messageAddress) {
            if (messageAddress == _currentMessageAddress || messageAddress == _errorMessageAddress) {
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
            return messageAddress == _currentMessageAddress;
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
