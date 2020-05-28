#include "logger.h"
#include "../assert_dev.h"
#include "../exception.h"
#include "adapter_defines.h"
#include <mutex>
#include <ostream>
#include <utility>

namespace UtilityBox {
    namespace Logger {
        // default adapters
        class StandardOutputAdapter final : public Adapter {
            public:
                explicit StandardOutputAdapter(std::string&& name, std::ostream* stream);
                ~StandardOutputAdapter() override;

                void ProcessMessage(void* messageAddress) override;
                void OutputMessage() override;

            private:
                std::ostream* _stream;
                std::vector<std::string> _formattedMessages;
                std::stringstream _format;
        };

        StandardOutputAdapter::StandardOutputAdapter(std::string&& name, std::ostream* stream) : Adapter(std::move(name)), _stream(stream) {
        }

        void StandardOutputAdapter::ProcessMessage(void *messageAddress) {
            // format:
            // [0000 0000] | DAY 00, MONTH 0000 | [ SEVERITY ]
            //      [000m 00s 0000ms] - Message goes here
            //          : supplied from (filename, function:000)

            // format message header
            const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);
            if (messageSeverity >= _config.severityCutoff) {
                const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance().GetLogRecords(messageAddress);
                // get timestamp length
                _format << TAB_SPACE << FormatTimestamp(messageRecords.at(0)._timestamp) << " - ";
                int timestampLength = _format.str().length();
                _format.str(std::string());

                _format << FormatLine(timestampLength + _config.messageWrapLimit) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                _format << FormatLogCounter() << " | " << FormatCalendarInformation() << " | " << FormatSeverity(messageSeverity) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());


                _format << TAB_SPACE << "Logged through system: " << LoggingHub::GetInstance().GetThroughLoggingSystem(messageAddress) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                _format << FormatLine(timestampLength + _config.messageWrapLimit) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                for (auto& logRecord : messageRecords) {
                    _format << TAB_SPACE << FormatTimestamp(logRecord._timestamp) << " - ";
                    for (std::string& message : FormatMessage(logRecord._message, timestampLength, _config.messageWrapLimit)) {
                        _format << message << std::endl;
                    }

#ifdef DEBUG_MESSAGES
                    _format << FormatDebugInformation(logRecord._calleeInformation) << std::endl;
#endif
                    _format << '\n';
                    _formattedMessages.emplace_back(_format.str());
                    _format.str(std::string());
                }

                ++_logCount;
            }
        }

        void StandardOutputAdapter::OutputMessage() {
            for (auto& message : _formattedMessages) {
                (*_stream) << message;
            }

            // add gaps between consecutive messages
            (*_stream) << "\n";
            _formattedMessages.clear();
        }

        StandardOutputAdapter::~StandardOutputAdapter() {
            _formattedMessages.clear();
        }

        LoggingHub* LoggingHub::_loggingHub = nullptr;

        class LoggingHub::LoggingHubData {
            public:
                void TerminateWorkerThread();
                void AttachAdapter(Adapter* adapter);
                Adapter* GetAdapter(const std::string& name);
                void Emplace(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity, const std::string& systemName);
                const std::vector<LogMessage::LogRecord>& GetLogRecords(void *messageAddress);
                const LogMessageSeverity& GetMessageSeverity(void *messageAddress);
                const std::string& GetThroughLoggingSystem(void *messageAddress);
                const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingHubInitializationTimestamp();

                explicit LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp);
                ~LoggingHubData();

            private:
                void DistributeMessages();
                void SwitchBuffers();

                std::vector<Adapter*> _adapters;
                std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime;

                struct LogMessageStorage {
                    LogMessageStorage(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity, std::string systemName);
                    virtual ~LogMessageStorage();
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
                std::thread _distributingThread;
        };

        LoggingHub::LoggingHubData::LogMessageStorage::LogMessageStorage(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity, std::string systemName) : _records(std::move(records)), _messageSeverity(messageSeverity), _loggedSystemName(std::move(systemName)) {
        }

        LoggingHub::LoggingHubData::LogMessageStorage::~LogMessageStorage() {
            _records.clear();
        }

        // starts an asynchronous worker thread working on the Distribute function once per asynchronousInterval. Uses _distributeMessages as a toggle.
        LoggingHub::LoggingHubData::LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp) : _initializationTime(initTimestamp), _asynchronousInterval(std::chrono::milliseconds(100)), _distributeMessages(false), _printingBufferLocation(&_printingBuffer1), _distributingThread(std::thread(&LoggingHub::LoggingHubData::DistributeMessages, this)) {
            // standard output (cout) and error (cerr) adapters
            auto* stdcout = new StandardOutputAdapter("Standard Output", &std::cout);
            stdcout->GetConfiguration().severityCutoff = LogMessageSeverity::DEBUG;
            auto* stdcerr = new StandardOutputAdapter("Standard Error", &std::cerr);
            stdcerr->GetConfiguration().severityCutoff = LogMessageSeverity::SEVERE;
            AttachAdapter(stdcout);
            AttachAdapter(stdcerr);

            // log file adapter
            auto* logFile = new FileAdapter("File Adapter");
            logFile->GetConfiguration().severityCutoff = LogMessageSeverity::DEBUG;
            AttachAdapter(logFile);

            // start thread after initialization
            _distributeMessages = true;
        }

        void LoggingHub::LoggingHubData::DistributeMessages() {
            // yield thread performance until logging hub is ready
            while (!_distributeMessages) {
                std::this_thread::yield();
            }

            while (_distributeMessages) {
                _bufferMutex.lock();
                std::queue<LogMessageStorage>& buffer = *_printingBufferLocation;
                SwitchBuffers();
                _bufferMutex.unlock();

                // data will now be written to other buffer while this one is being printed from
                while (!buffer.empty()) {
                    LogMessageStorage& packet = buffer.front();
                    for (auto* adapter : _adapters) {
                        try {
                            adapter->ProcessMessage(static_cast<void*>(&packet));
                            adapter->OutputMessage();
                        }
                        catch (const Exceptions::Exception& exception) {

                        }
                    }
                    buffer.pop();
                }
                std::this_thread::sleep_for(_asynchronousInterval);
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
            // wait for both buffers to be completely empty
            while (!_printingBuffer1.empty() || !_printingBuffer2.empty()) {
                std::this_thread::yield();
            }

            // send 'shutdown' message to worker thread
            _distributeMessages = false;

            // pause the main thread to ensure worker thread has enough time to process 'shutdown' message
            _distributingThread.join();

            for (auto* adapter : _adapters) {
                delete adapter;
            }
            _adapters.clear();
        }

        void LoggingHub::LoggingHubData::AttachAdapter(Adapter *adapter) {
            _adapters.emplace_back(adapter);
        }

        void LoggingHub::LoggingHubData::Emplace(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity, const std::string& systemName) {
            (*_printingBufferLocation).emplace(std::move(records), messageSeverity, systemName);
        }

        const std::vector<LogMessage::LogRecord> &LoggingHub::LoggingHubData::GetLogRecords(void *messageAddress) {
            auto* dataPacketBasePointer = static_cast<LogMessageStorage*>(messageAddress);
            if (auto* dataPacket = dynamic_cast<LogMessageStorage*>(dataPacketBasePointer)) {
                return dataPacket->_records;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageTimestamps from custom adapter");
        }

        const LogMessageSeverity &LoggingHub::LoggingHubData::GetMessageSeverity(void *messageAddress) {
            auto* dataPacketBasePointer = static_cast<LogMessageStorage*>(messageAddress);
            if (auto* dataPacket = dynamic_cast<LogMessageStorage*>(dataPacketBasePointer)) {
                return dataPacket->_messageSeverity;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageTimestamps from custom adapter");
        }

        const std::string& LoggingHub::LoggingHubData::GetThroughLoggingSystem(void *messageAddress) {
            auto* dataPacketBasePointer = static_cast<LogMessageStorage*>(messageAddress);
            if (auto* dataPacket = dynamic_cast<LogMessageStorage*>(dataPacketBasePointer)) {
                return dataPacket->_loggedSystemName;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageTimestamps from custom adapter");
        }

        const std::chrono::time_point<std::chrono::high_resolution_clock>& LoggingHub::LoggingHubData::GetLoggingHubInitializationTimestamp() {
            return _initializationTime;
        }

        Adapter *LoggingHub::LoggingHubData::GetAdapter(const std::string &name) {
            for (auto* adapter : _adapters) {
                if (adapter->GetName() == name) {
                    return adapter;
                }
            }

            return nullptr;
        }

        void LoggingHub::LoggingHubData::TerminateWorkerThread() {

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


    }
}
