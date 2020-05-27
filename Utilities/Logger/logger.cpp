#include "logger.h"
#include "../assert_dev.h"
#include "../exception.h"
#include <mutex>
#include <ostream>

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
                _format << FormatLogCounter() << " | " << FormatCalendarInformation() << " | " << FormatSeverity(messageSeverity) << std::endl;
                _formattedMessages.emplace_back(_format.str());
                _format.str(std::string());

                // format message body
                const std::vector<LogMessage::LogRecord>& messageRecords = LoggingHub::GetInstance().GetLogRecords(messageAddress);
                for (auto& message : messageRecords) {
                    _format << '\t' << FormatTimestamp(message._timestamp) << " - " << message._message << std::endl;
    #ifdef DEBUG_MESSAGES
                    _format << "\t\t" << ": supplied from (" << message._calleeInformation._fileName << ", " << message._calleeInformation._functionName << ':' << message._calleeInformation._lineNumber << ')' << std::endl;
    #endif
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
            (*_stream) << std::endl;
        }

        StandardOutputAdapter::~StandardOutputAdapter() {
            _formattedMessages.clear();
        }

        LoggingHub* LoggingHub::_loggingHub = nullptr;

        class LoggingHub::LoggingHubData {
            public:
                const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingHubInitializationTimestamp();
                void AttachAdapter(Adapter* adapter);
                Adapter* GetAdapter(const std::string& name);
                void Emplace(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity);
                const std::vector<LogMessage::LogRecord>& GetLogRecords(void *messageAddress);
                const LogMessageSeverity& GetMessageSeverity(void *messageAddress);

                explicit LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp);
                ~LoggingHubData();

            private:
                void DistributeMessages();
                void SwitchBuffers();

                LoggingSystem* _hubLoggingSystem;
                std::vector<Adapter*> _adapters;
                std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime;

                struct LogMessageStorage {
                    LogMessageStorage(std::vector<LogMessage::LogRecord>&& records, LogMessageSeverity messageSeverity);
                    virtual ~LogMessageStorage();
                    std::vector<LogMessage::LogRecord> _records;
                    LogMessageSeverity _messageSeverity;
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

        LoggingHub::LoggingHubData::LogMessageStorage::LogMessageStorage(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity) : _records(std::move(records)), _messageSeverity(messageSeverity) {
        }

        LoggingHub::LoggingHubData::LogMessageStorage::~LogMessageStorage() {
            _records.clear();
        }

        // starts an asynchronous worker thread working on the Distribute function once per asynchronousInterval. Uses _distributeMessages as a toggle.
        LoggingHub::LoggingHubData::LoggingHubData(const std::chrono::time_point<std::chrono::high_resolution_clock>& initTimestamp) : _hubLoggingSystem(new LoggingSystem("Logging Hub")), _initializationTime(initTimestamp), _asynchronousInterval(std::chrono::milliseconds(100)), _distributeMessages(true), _distributingThread(std::move(std::thread(&LoggingHub::LoggingHubData::DistributeMessages, this))), _printingBufferLocation(&_printingBuffer1) {
            auto* stdcout = new StandardOutputAdapter("Standard Output", &std::cout);
            stdcout->GetConfiguration().severityCutoff = LogMessageSeverity::DEBUG;
            auto* stdcerr = new StandardOutputAdapter("Standard Error", &std::cerr);
            stdcerr->GetConfiguration().severityCutoff = LogMessageSeverity::SEVERE;

            AttachAdapter(stdcout);
            AttachAdapter(stdcerr);
        }

        void LoggingHub::LoggingHubData::DistributeMessages() {
            if (_distributeMessages) {
                // lock this area so only one thread can access it
                // lock guard automatically gets unlocked when lock goes out of scope
                std::cout << "before lock" << std::endl;
                std::lock_guard<std::mutex> lock(_bufferMutex);

                std::cout << "entered lock" << std::endl;
                // redirect data flow to the other buffer
                void* location = _printingBufferLocation;
                unsigned size = (*_printingBufferLocation).size();
                std::cout << "size at location: " << location << " : " << size << std::endl;

                if (!(*_printingBufferLocation).empty()) {
                    // get the buffer to empty
                    std::queue<LogMessageStorage>& buffer = *_printingBufferLocation;

                    // data will now be written to other buffer while this one is being printed from
                    SwitchBuffers();

                    while (!buffer.empty()) {
                        LogMessageStorage& packet = buffer.front();
                        std::cout << "emptying" << std::endl;

                        for (auto* adapter : _adapters) {
                            try {
                                adapter->ProcessMessage(static_cast<void*>(&packet));
                                adapter->OutputMessage();
                            }
                            catch (const Exceptions::Exception& exception) {
                                std::cout << exception.what() << std::endl;
                                // todo: better
                            }
                        }

                        buffer.pop();
                        std::cout << "size at location: " << &buffer << " : " << buffer.size() << std::endl << std::endl;
                    }
                }
            }

            std::this_thread::sleep_for(_asynchronousInterval);
        }

        void LoggingHub::LoggingHubData::SwitchBuffers() {
            if (_printingBufferLocation == &(_printingBuffer1)) {
                std::cout << "switching from " << &_printingBuffer1 << " to " << &_printingBuffer2 << std::endl;
                std::cout << "current active buffer is " << &_printingBuffer2 << std::endl;
                std::cout.flush();
                _printingBufferLocation = &_printingBuffer2;
            }
            else {
                std::cout << "switching from " << &_printingBuffer2 << " to " << &_printingBuffer1 << std::endl;
                std::cout << "current active buffer is " << &_printingBuffer1 << std::endl << std::endl;
                std::cout.flush();
                _printingBufferLocation = &_printingBuffer1;
            }
        }

        LoggingHub::LoggingHubData::~LoggingHubData() {
            std::cout << "destructor for logging hub called" << std::endl;

            // wait for both buffers to be completely empty
            if (!_printingBuffer1.empty() || !_printingBuffer2.empty()) {
                std::cout << "buffers not empty, yielding" << std::endl;
                std::this_thread::yield();
            }

            // send 'shutdown' message to worker thread
            _distributeMessages = false;

            // pause the main thread to ensure worker thread has enough time to process 'shutdown' message
            std::cout << "waiting for thread" << std::endl;
            _distributingThread.join();
            std::cout.flush();
            std::cout << "thread finished" << std::endl;

            for (auto* adapter : _adapters) {
                delete adapter;
            }
            _adapters.clear();

            delete _hubLoggingSystem;
        }

        void LoggingHub::LoggingHubData::AttachAdapter(Adapter *adapter) {
            _adapters.emplace_back(adapter);
        }

        void LoggingHub::LoggingHubData::Emplace(std::vector<LogMessage::LogRecord> &&records, LogMessageSeverity messageSeverity) {
            (*_printingBufferLocation).emplace(std::move(records), messageSeverity);
        }

        const std::vector<LogMessage::LogRecord> &LoggingHub::LoggingHubData::GetLogRecords(void *messageAddress) {
            auto* dataPacketBasePointer = static_cast<LogMessageStorage*>(messageAddress);
            if (auto* dataPacket = dynamic_cast<LogMessageStorage*>(dataPacketBasePointer)) {
                std::cout << "successful cast, data is accessed" << std::endl;
                return dataPacket->_records;
            }

            throw Exceptions::Exception("Invalid pointer provided to GetMessageTimestamps from custom adapter");
        }

        const LogMessageSeverity &LoggingHub::LoggingHubData::GetMessageSeverity(void *messageAddress) {
            auto* dataPacketBasePointer = static_cast<LogMessageStorage*>(messageAddress);
            if (auto* dataPacket = dynamic_cast<LogMessageStorage*>(dataPacketBasePointer)) {
                std::cout << "successful cast, data is accessed" << std::endl;
                return dataPacket->_messageSeverity;
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

        // Begin logger functions
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
            _loggingHub->_data.reset();
            delete _loggingHub;
        }

        Logger::LoggingHub::LoggingHub() : _data(std::move(std::make_unique<LoggingHubData>(std::chrono::high_resolution_clock::now()))) {
            // nothing
        }

        Logger::LoggingHub::~LoggingHub() {
            _data.reset();
            _loggingHub = nullptr;
        }

        void Logger::LoggingHub::SendMessage(LogMessage &&message) {
            _data->Emplace(std::move(*message.GetLogMessages()), message.GetMessageSeverity());
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
        const std::chrono::time_point<std::chrono::high_resolution_clock> &LoggingHub::GetLoggingInitializationTime() {
            return _data->GetLoggingHubInitializationTimestamp();
        }

        Adapter* LoggingHub::GetCustomAdapter(const std::string &name) {
            return _data->GetAdapter(name);
        }
    }
}
