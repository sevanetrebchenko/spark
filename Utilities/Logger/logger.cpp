
#ifndef DATASTRUCTURES_LOGGER_CPP
#define DATASTRUCTURES_LOGGER_CPP

#include "logger.h"
#include <utility>
#include <vector>
#include <bitset>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>

namespace UtilityBox::Logger {
    // LogMessageData begin
    class LogMessage::LogMessageData {
        public:
            explicit LogMessageData(LogMessageSeverity messageSeverity);
            ~LogMessageData();
            void ProcessMessage(const char* formatString, std::va_list argList);

            std::vector<std::string> _messageData;
            std::vector<TimeStamp::TimeStamp> _timestampData;
            LogMessageSeverity _messageSeverity;

        private:
            unsigned _processingBufferSize;
            char* _processingBuffer;
    };

    LogMessage::LogMessageData::LogMessageData(LogMessageSeverity messageSeverity) : _processingBufferSize(64u), _messageSeverity(messageSeverity) {
        _processingBuffer = new (std::nothrow) char[_processingBufferSize];
        // TODO: assert
}

    LogMessage::LogMessageData::~LogMessageData() {
        delete [] _processingBuffer;
        // TODO: _formattedMessages.clear();
    }

    void LogMessage::LogMessageData::ProcessMessage(const char* formatString, std::va_list argList) {
        // record timestamp
        _timestampData.emplace_back();

        // calculate correct number of bytes to write
        unsigned currentBufferSize = _processingBufferSize;

        // copy args list to not modify passed parameters
        std::va_list argsCopy;
        va_copy(argsCopy, argList);
        // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
        int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

        // get buffer size of powers of 2 to match the size correctly
        // if buffer size is equal to write result, there will not be space for the null terminator for the string
        while (_processingBufferSize <= writeResult) {
            _processingBufferSize *= 2;
        }

        // reallocate buffer
        if (currentBufferSize != _processingBufferSize) {
            delete [] _processingBuffer;
            _processingBuffer = new char[_processingBufferSize];
        }

        // write data to buffer
        vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);

        _messageData.emplace_back(_processingBuffer);
    }
    // LogMessageData end

    // DataPacket begin
    struct DataPacket {
        DataPacket(LogMessageSeverity messageSeverity, unsigned logCount, std::vector<std::string> &&messages, std::vector<TimeStamp::TimeStamp>&& timestamps, std::string logname);
        DataPacket(DataPacket&& other) noexcept;
        ~DataPacket();
        std::vector<std::string> _messageData;
        std::vector<TimeStamp::TimeStamp> _timestampData;
        LogMessageSeverity _messageSeverity;
//        std::bitset<NUM_TAGS> a;
        std::string _loggingSystemName;
        unsigned _logCount;
    };

    DataPacket::DataPacket(LogMessageSeverity messageSeverity, unsigned int logCount, std::vector<std::string> &&messages, std::vector<TimeStamp::TimeStamp>&& timestamps,  std::string logname) : _messageSeverity(messageSeverity), _logCount(logCount), _messageData(std::move(messages)), _timestampData(std::move(timestamps)), _loggingSystemName(std::move(logname)) {
    }

    DataPacket::DataPacket(DataPacket&& other) noexcept {
        _messageData = std::move(other._messageData);
        _timestampData = std::move(other._timestampData);
        _loggingSystemName = other._loggingSystemName;
        _messageSeverity = other._messageSeverity;
        _logCount = other._logCount;
    }

    DataPacket::~DataPacket() {
        std::cout << "destroying data packet" << std::endl;
    }
    // DataPacket end

    // LoggingSystemData begin
    struct LoggingSystem::LoggingSystemData {
        explicit LoggingSystemData(std::string name);

        std::string _systemName;
        unsigned _logCounter;
    };

    LoggingSystem::LoggingSystemData::LoggingSystemData(std::string name) : _systemName(std::move(name)), _logCounter(0) {
    }
    // LoggingSystemData end

    // LoggingHub begin
    class LoggingHub {
        public:
            static LoggingHub* GetLoggingHubInstance();
            static LoggingSystem* GetLoggingSystemInstance();

            LoggingHub();
            ~LoggingHub();

            bool& IsInitialized();
            void AddCustomAdapter(Adapter* adapter);
            void Emplace(DataPacket&& data);
            void Distribute();

            _NODISCARD_ bool VerifyMessageAddress(void* messageAddress);

        private:
            void SwitchBuffers();

            bool _initializationToggle;

            // threading
            std::mutex _bufferMutex;
            std::chrono::milliseconds _asynchronousInterval;
            std::atomic<bool> _distributeMessages;
            std::thread _distributingThread;

            // buffers
            std::queue<DataPacket> _printingBuffer1;
            std::queue<DataPacket> _printingBuffer2;
            std::queue<DataPacket> _printingError;

            // abstraction to allow buffer swapping
            std::queue<DataPacket>* _printingBufferLocation;
            void* _currentPacketAddress;

            std::vector<Adapter*> _adapters;
    };

    LoggingHub *LoggingHub::GetLoggingHubInstance() {
        // constructs a singleton logging hub if it hasn't been already
        static const std::unique_ptr<LoggingHub> loggingHub = std::make_unique<LoggingHub>();
        return loggingHub.get();
    }

    void LoggingHub::Emplace(DataPacket&& data) {
        // call move constructor and emplace into queue
        _printingBufferLocation->emplace(std::move(data));
        std::cout << "writing to " << &*_printingBufferLocation << std::endl;
    }

    // starts an asynchronous worker thread working on the Distribute function once per asynchronousInterval. Uses _distributeMessages as a toggle.
    LoggingHub::LoggingHub() : _initializationToggle(false), _asynchronousInterval(std::chrono::milliseconds(200)), _distributeMessages(true), _distributingThread(std::thread(&LoggingHub::Distribute, this)), _printingBufferLocation(&_printingBuffer1), _currentPacketAddress(nullptr) {
    }

    void LoggingHub::Distribute() {
        while (_distributeMessages) {

            // lock this area so only one thread can access it
            // lock guard automatically gets unlocked when lock goes out of scope
            _bufferMutex.lock();
            // asynchronous code here

            std::cout << "entered lock" << std::endl;
            // redirect data flow to the other buffer
            std::cout << "size at location: " << *&_printingBufferLocation << " : " << _printingBufferLocation->size() << std::endl;

            if (!_printingBufferLocation->empty()) {
                // get the buffer to empty
                std::queue<DataPacket>& buffer = *_printingBufferLocation;

                // data will now be written to other buffer while this one is being printed from
                SwitchBuffers();

                while (!buffer.empty()) {
                    DataPacket& packet = buffer.front();
                    _currentPacketAddress = static_cast<void*>(&packet);

                    for (auto* adapter : _adapters) {
                        try {
                            adapter->ProcessMessage(_currentPacketAddress);
                            adapter->OutputMessages();
                        }
                        catch (const Exceptions::Exception& exception) {
                            std::cout << exception.what() << std::endl;
                            // todo: better
                        }

                    buffer.pop();
                    }
                }

                _bufferMutex.unlock();
            }

            std::cout << "size at location: " << *&_printingBufferLocation << " : " << _printingBufferLocation->size() << std::endl << std::endl;
            std::this_thread::sleep_for(_asynchronousInterval);
        }
    }

    LoggingHub::~LoggingHub() {
        // send 'shutdown' message to worker thread
        _distributeMessages = false;

        // pause the main thread to ensure worker thread has enough time to process 'shutdown' message
        std::this_thread::sleep_for(_asynchronousInterval);
        _distributingThread.join();

        if (!_printingBuffer1.empty() || !_printingBuffer2.empty()) {
            //DistributeRemainder();
        }
    }

    void LoggingHub::AddCustomAdapter(Adapter *adapter) {
        _adapters.push_back(adapter);
    }

    void LoggingHub::SwitchBuffers() {
        if (&(*_printingBufferLocation) == &(_printingBuffer1)) {
            std::cout << "switching from " << &_printingBuffer1 << " to " << &_printingBuffer2 << std::endl;
            std::cout << "current active buffer is " << &_printingBuffer2 << std::endl;
            _printingBufferLocation = &_printingBuffer2;
        }
        else {
            std::cout << "switching from " << &_printingBuffer2 << " to " << &_printingBuffer1 << std::endl;
            std::cout << "current active buffer is " << &_printingBuffer1 << std::endl << std::endl;
            _printingBufferLocation = &_printingBuffer1;
        }
    }

    bool LoggingHub::VerifyMessageAddress(void* messageAddress) {
        return reinterpret_cast<std::intptr_t>(messageAddress) == reinterpret_cast<std::intptr_t>(_currentPacketAddress);
    }

    LoggingSystem *LoggingHub::GetLoggingSystemInstance() {
        // constructs a singleton logging system for this logging hub if it hasn't been already
        static const std::unique_ptr<LoggingSystem> loggingSystem = std::make_unique<LoggingSystem>("Logging Hub");
        return loggingSystem.get();
    }

    bool& LoggingHub::IsInitialized() {
        return _initializationToggle;
    }
    // LoggingHub end

    // LogMessage functions begin
    LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _data(std::make_unique<LogMessageData>(messageSeverity)) {
    }

    LogMessage::LogMessage(LogMessageSeverity messageSeverity, const char* formatString, ...) : _data(std::make_unique<LogMessageData>(messageSeverity)){
        // _data is guaranteed to be initialized when the constructor body code is executed
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }

    LogMessage::~LogMessage() {
        _data.reset();
    }

    void LogMessage::Supply(const char* formatString, ...) {
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }

    std::ostream &operator<<(std::ostream &stream, const LogMessage &message) {
        //stream << message._data. << std::endl;
        return stream;
    }
    // LogMessage functions end


    // LoggingSystem functions begin
    LoggingSystem::LoggingSystem(std::string&& name) : _data(std::make_unique<LoggingSystemData>(std::move(name))){
        // nothing to do here
    }

    LoggingSystem::~LoggingSystem() {
        _data.reset();
    }

    void LoggingSystem::Log(LogMessage* message) {
        InitializeLoggingSystem();

        LoggingHub::GetLoggingHubInstance()->Emplace(DataPacket(message->_data->_messageSeverity, _data->_logCounter, std::move(message->_data->_messageData), std::move(message->_data->_timestampData), _data->_systemName));
        delete message;
    }

    void LoggingSystem::Log(LogMessageSeverity messageSeverity, const char *formatString, ...) {
        // create message instance
        auto* message = new LogMessage(messageSeverity);

        // TODO: optimize?
        std::va_list args;
        va_start(args, formatString);
        message->Supply(formatString, args);
        va_end(args);

        Log(message);
    }
    // LoggingSystem functions end


    class Adapter::AdapterData {
        public:
            explicit AdapterData(std::string name);
            void AppendMessage(std::string message);

            std::string _name;
        private:
            std::vector<std::string> _formattedMessages;
    };

    Adapter::Adapter(std::string name) : _data(std::make_unique<AdapterData>(std::move(name))) {
    }

    Adapter::AdapterData::AdapterData(std::string name) : _name(std::move(name)) {
    }

    void Adapter::AdapterData::AppendMessage(std::string message) {
        _formattedMessages.emplace_back(std::move(message));
    }

    const std::vector<TimeStamp::TimeStamp> *Adapter::GetMessageTimestamps(void *messageAddress) {
        if (LoggingHub::GetLoggingHubInstance()->VerifyMessageAddress(messageAddress)) {
            auto* dataPacket = static_cast<DataPacket*>(messageAddress);
            return &dataPacket->_timestampData;
        }

        throw Exceptions::Exception("Invalid pointer provided to GetMessageTimestamps from custom adapter %s.", _data->_name.c_str());
    }

    const std::vector<std::string>* Adapter::GetMessageLogs(void* messageAddress) {
        if (LoggingHub::GetLoggingHubInstance()->VerifyMessageAddress(messageAddress)) {
            auto* dataPacket = static_cast<DataPacket*>(messageAddress);
            return &dataPacket->_messageData;
        }

        throw Exceptions::Exception("Invalid pointer provided to GetMessageLogs from custom adapter %s.", _data->_name.c_str());
    }

    LogMessageSeverity Adapter::GetMessageSeverity(void* messageAddress) {
        if (LoggingHub::GetLoggingHubInstance()->VerifyMessageAddress(messageAddress)) {
            auto* dataPacket = static_cast<DataPacket*>(messageAddress);
            return dataPacket->_messageSeverity;
        }

        throw Exceptions::Exception("Invalid pointer provided to GetMessageSeverity from custom adapter %s.", _data->_name.c_str());
    }

    const std::string* Adapter::GetMessageLoggingSystemName(void* messageAddress) {
        if (LoggingHub::GetLoggingHubInstance()->VerifyMessageAddress(messageAddress)) {
            auto* dataPacket = static_cast<DataPacket*>(messageAddress);
            return &dataPacket->_loggingSystemName;
        }

        throw Exceptions::Exception("Invalid pointer provided to GetMessageLoggingSystemName from custom adapter %s.", _data->_name.c_str());
    }

    const unsigned* Adapter::GetMessageLogCount(void* messageAddress) {
        if (LoggingHub::GetLoggingHubInstance()->VerifyMessageAddress(messageAddress)) {
            auto* dataPacket = static_cast<DataPacket*>(messageAddress);
            return &dataPacket->_logCount;
        }

        throw Exceptions::Exception("Invalid pointer provided to GetMessageLogCount from custom adapter %s.", _data->_name.c_str());
    }

    Adapter::~Adapter() {
        _data.reset();
    }

    void AttachCustomAdapter(Adapter* adapter) {
        LoggingHub::GetLoggingHubInstance()->AddCustomAdapter(adapter);
    }


    // default adapters
    class StandardOutputAdapter final : public Adapter {
        public:
            StandardOutputAdapter();
            ~StandardOutputAdapter() override;

            void ProcessMessage(void* messageData) override;
            void OutputMessages() override;
        private:
            std::vector<std::string> _formattedMessages;
    };

    StandardOutputAdapter::StandardOutputAdapter() : Adapter("Standard Output") {
    }

    void StandardOutputAdapter::ProcessMessage(void *messageData) {
        auto* timestamps = GetMessageTimestamps(messageData);
        auto* messages = GetMessageLogs(messageData);
        auto* newptr = static_cast<int*>(messageData);
        newptr++;

        auto* logSystemName = GetMessageLoggingSystemName(static_cast<void*>(newptr));
//        auto* logSystemName = GetMessageLoggingSystemName(messageData);
        auto* logCount = GetMessageLogCount(messageData);
        LogMessageSeverity severity = GetMessageSeverity(messageData);

        std::stringstream stream;
        stream << "log number: " << logCount << ", logging system name: " << logSystemName << std::endl;
        _formattedMessages.emplace_back(stream.str());
        stream.str(std::string());

        unsigned size = messages->size();

        for (unsigned i = 0; i < size; ++i) {
            stream << "timestamp: " << (*timestamps)[i].ConvertToMillis() << " message: " << (*messages)[i] << std::endl;
            _formattedMessages.emplace_back(stream.str());
            stream.str(std::string());
        }
    }

    void StandardOutputAdapter::OutputMessages() {
        for (auto& message : _formattedMessages) {
            std::cout << message;
        }
    }

    StandardOutputAdapter::~StandardOutputAdapter() {
        _formattedMessages.clear();
    }

    void InitializeLoggingSystem() {
        // construct logging hub instance
        // prevents statement from being entered a second time
        bool& initialized = LoggingHub::GetLoggingHubInstance()->IsInitialized();
        if (!initialized) {
            initialized = true;
            LoggingHub::GetLoggingHubInstance()->AddCustomAdapter(new StandardOutputAdapter());
            auto* hubInitMessage = new LogMessage(DEBUG, "Initializing logging systems.");
            LoggingHub::GetLoggingSystemInstance()->Log(hubInitMessage);
        }
    }

}

#endif

//namespace UtilityBox {
//    namespace Logger {
//
//        typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestampType;
//
//        struct LogMessageData {
//            LogMessageData() = default;
//            std::string _messageFormatted;      // Formatted log message
//            std::string _logNumberFormatted;    // Log number
//            std::string _calendarDateFormatted; // Calendar date and local time of this log
//            std::string _severityFormatted;     // Formatted log severity
//            std::string _timestampFormatted;    // Timestamp of this log since the log file was open
//
//            /**
//             * Print the formatted log to the given stream
//             * @param stream Stream to print to
//             * @param message Log message to print
//             * @return Stream that was printed to
//             */
//            friend std::ostream& operator<<(std::ostream &stream, const LogMessageData &message);
//        };
//
//        class LogMessageBackEnd {
//            public:
//                LogMessageBackEnd();
//                ~LogMessageBackEnd();
//                void ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args);
//                void Clear();
//
//                friend std::ostream& operator<<(std::ostream& stream, const LogMessageBackEnd& storage);
//
//            private:
//                // formatting functions
//                _NODISCARD_ std::string FormatMessage(const char* formatString, std::va_list args);
//                _NODISCARD_ std::string FormatLogNumber();
//                _NODISCARD_ std::string FormatCalendarDate();
//                _NODISCARD_ std::string FormatMessageSeverity(LogMessageSeverity severity);
//                _NODISCARD_ std::string FormatTimestamp(timestampType timestamp);
//
//                std::stringstream _format;
//
//                char* _logBuffer;
//                char* _calendarBuffer;
//
//                unsigned _logBufferSize;
//                unsigned _calendarBufferSize;
//                std::time_t _calendarTime;
//                bool _isMultiLine;
//
//                std::vector<LogMessageData> _messages;
//        };
//
//        struct LogMessageBackEndHeader {
//            LogMessageBackEndHeader(bool inUse, LogMessageBackEnd* data);
//            bool _inUse;
//            LogMessageBackEnd* _data;
//        };
//
//        class LoggingSystem {
//            public:
//                _NODISCARD_ static LoggingSystem *GetLoggingHubInstance();
//                _NODISCARD_ unsigned ConstructMessageStorage();
//                _NODISCARD_ LogMessageBackEndHeader & GetBackEnd(unsigned ID);
//                _NODISCARD_ UtilityBox::Logger::LoggerConfiguration& GetConfig();
//                _NODISCARD_ unsigned GetLogCount();
//                _NODISCARD_ timestampType GetLogStartTime();
//
//                void Log(SingleLineMessage*);
//                void Log(MultiLineMessage*);
//                void Log(unsigned);
//
//            private:
//                // SINGLETON
//                static LoggingSystem* _loggingSystem;
//
//                // FUNCTIONS
//                LoggingSystem();
//                ~LoggingSystem();
//
//                // LoggingSystem data
//                UtilityBox::Logger::LoggerConfiguration _config;
//                timestampType _loggerStartTime;
//                std::ofstream _logger;
//                std::string _logFile;
//                unsigned _logCount;
//
//                std::vector<LogMessageBackEndHeader> _messages;
//
//        };
//
//        LogMessageBackEndHeader::LogMessageBackEndHeader(bool inUse, LogMessageBackEnd *data) : _inUse(inUse), _data(data) {
//        }
//
//        LoggingSystem* LoggingSystem::_loggingSystem = nullptr;
//
//        LoggingSystem *LoggingSystem::GetLoggingHubInstance() {
//            if (!_loggingSystem) {
//                _loggingSystem = new LoggingSystem();
//
//                // check the open status of the log file
//                _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
//            }
//
//            return _loggingSystem;
//        }
//
//        void LoggingSystem::Log(SingleLineMessage* message) {
//            _logger << *message;
//        }
//
//        void LoggingSystem::Log(MultiLineMessage *message) {
//            _logger << *message;
//        }
//
//        void LoggingSystem::Log(unsigned ID) {
//            _logger << *(_messages.at(ID)._data);
//        }
//
//        LoggingSystem::LoggingSystem() {
//            _loggerStartTime = std::chrono::high_resolution_clock::now();
//            _logFile = "log2.txt";
//            _logCount = 0;
//        }
//
//        LoggingSystem::~LoggingSystem() {
//            _loggingSystem->_logger.close();
//        }
//
//        UtilityBox::Logger::LoggerConfiguration &LoggingSystem::GetConfig() {
//            return _config;
//        }
//
//        unsigned LoggingSystem::GetLogCount() {
//            return _logCount++;
//        }
//
//        timestampType LoggingSystem::GetLogStartTime() {
//            return _loggerStartTime;
//        }
//
//        unsigned LoggingSystem::ConstructMessageStorage() {
//            unsigned size = _messages.size();
//
//            // find an already existing header that has been cleared and is not in use
//            for (unsigned i = 0; i < size; ++i) {
//                auto& header = _messages.at(i);
//
//                if (!header._inUse) {
//                    header._inUse = true;
//                    return i;
//                }
//            }
//
//            // all headers are taken, create a new one
//            _messages.emplace_back(true, new LogMessageBackEnd());
//            return size;
//        }
//
//        LogMessageBackEndHeader& LoggingSystem::GetBackEnd(unsigned int ID) {
//            return _messages.at(ID);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const LogMessageData &message) {
//            const LoggerConfiguration& config = LoggingSystem::GetLoggingHubInstance()->GetConfig();
//
//            if (config.includeMessageNum) {
//                stream << message._logNumberFormatted.c_str() << " ";
//            }
//
//            if (config.includeDateTime) {
//                stream << message._calendarDateFormatted.c_str() << " ";
//            }
//
//            if (config.includeMessageTimestamp) {
//                stream << message._timestampFormatted.c_str() << " ";
//            }
//
//            if (config.includeMessageSeverity) {
//                stream << message._severityFormatted.c_str() << " ";
//            }
//
//            stream << message._messageFormatted.c_str() << std::endl;
//
//            return stream;
//        }
//
//        std::string LogMessageBackEnd::FormatLogNumber() {
//            unsigned logMessageID = LoggingSystem::GetLoggingHubInstance()->GetLogCount();
//            _format << "[";
//            _format << std::setfill('0') << std::setw(3);
//            _format << logMessageID / 1000;
//
//            _format << " ";
//
//            _format << std::setfill('0') << std::setw(3);
//            _format << logMessageID % 1000;
//            _format << "]";
//
//            std::string logNumberFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//
//            return logNumberFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatCalendarDate() {
//            while (!std::strftime(_calendarBuffer, _calendarBufferSize, UtilityBox::Logger::GetLoggerConfig().calendarHeaderFormat.c_str(), std::localtime(&_calendarTime))) {
//                _calendarBufferSize *= 2;
//                delete[] _calendarBuffer;
//                _calendarBuffer = new char[_calendarBufferSize];
//            }
//
//            return _calendarBuffer;
//        }
//
//        std::string LogMessageBackEnd::FormatMessageSeverity(LogMessageSeverity messageSeverity) {
//            _format << "[";
//
//            // [  DEBUG  ]
//            // [ WARNING ]
//            // [  ERROR  ]
//            // [ UNKNOWN ]
//            switch (messageSeverity) {
//                case DEBUG:
//                    _format << "  DEBUG  ";
//                    break;
//                case WARNING:
//                    _format << " WARNING ";
//                    break;
//                case ERROR:
//                    _format << "  ERROR  ";
//                    break;
//                default:
//                    _format << " UNKNOWN ";
//                    break;
//            }
//
//            _format << "]";
//            std::string severityFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//            return severityFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatTimestamp(timestampType timestamp) {
//            // get the time in milliseconds
//            unsigned long elapsed = (timestamp - LoggingSystem::GetLoggingHubInstance()->GetLogStartTime()).count() / 1000;
//
//            // milliseconds
//            unsigned long milliseconds = elapsed;
//            // seconds
//            unsigned long seconds = elapsed / 1000;
//            // minutes
//            unsigned long minutes = elapsed / 60000;
//
//            // 000m 00s 0000ms [SEVERITY] MESSAGE
//            // append minutes
//            _format << std::setfill('0') << std::setw(3);
//            _format << minutes << "m ";
//
//            // append seconds
//            _format << std::setfill('0') << std::setw(2);
//            _format << seconds << "s ";
//
//            // append milliseconds
//            _format << std::setfill('0') << std::setw(4);
//            _format << milliseconds << "ms ";
//
//            std::string timestampFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//            return timestampFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatMessage(const char *formatString, std::va_list args) {

//        }
//
//        LogMessageBackEnd::LogMessageBackEnd() : _logBufferSize(256u), _calendarBufferSize(64u), _calendarTime(std::time(nullptr)), _isMultiLine(false) {
//            // allocate buffers
//            _logBuffer = new(std::nothrow) char[_logBufferSize];
//            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
//            //TODO: asserts for memory checks
//        }
//
//        LogMessageBackEnd::~LogMessageBackEnd() {
//            delete[] _logBuffer;
//            delete[] _calendarBuffer;
//        }
//
//        void LogMessageBackEnd::ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args) {
//            _messages.emplace_back();
//            LogMessageData& data = _messages.at(_messages.size() - 1);
//            data._messageFormatted = FormatMessage(formatString, args);
//            data._logNumberFormatted = FormatLogNumber();
//            data._severityFormatted = FormatMessageSeverity(messageSeverity);
//            data._calendarDateFormatted = FormatCalendarDate();
//            data._timestampFormatted = FormatTimestamp(timestamp);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const LogMessageBackEnd &storage) {
//            const LoggerConfiguration& config = LoggingSystem::GetLoggingHubInstance()->GetConfig();
//
//            for (auto& message : storage._messages) {
//                stream << message;
//            }
//
//            if (config.separateMessages) {
//                stream << std::endl << std::endl;
//            }
//
//            return stream;
//        }
//
//        void LogMessageBackEnd::Clear() {
//            memset(_logBuffer, 0, _logBufferSize);
//            memset(_calendarBuffer, 0, _calendarBufferSize);
//            _messages.clear();
//            _format.str(std::string());
//        }
//    }
//}
//
//
//// PUBLIC API DEFINITIONS
//namespace UtilityBox {
//    namespace Logger {
//        // LoggingSystem functions
//        void Log(SingleLineMessage *msg) {
//            return LoggingSystem::GetLoggingHubInstance()->Log(msg);
//        }
//
//        void Log(MultiLineMessage *msg) {
//            return LoggingSystem::GetLoggingHubInstance()->Log(msg);
//        }
//
//        void Log(LogMessageSeverity messageSeverity, const char* formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            unsigned constructedID = LoggingSystem::GetLoggingHubInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(constructedID);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//
//            LoggingSystem::GetLoggingHubInstance()->Log(constructedID);
//
//            storage._inUse = false;
//            storage._data->Clear();
//        }
//
//        LoggerConfiguration &GetLoggerConfig() {
//            return LoggingSystem::GetLoggingHubInstance()->GetConfig();
//        }
//
//        // Single-line message functions
//        SingleLineMessage::~SingleLineMessage() = default;
//
//        SingleLineMessage::SingleLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            _dataIndex = LoggingSystem::GetLoggingHubInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const SingleLineMessage &msg) {
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(msg._dataIndex);
//            stream << *(storage._data);
//            storage._inUse = false;
//            storage._data->Clear();
//            return stream;
//        }
//
//        unsigned SingleLineMessage::GetDataIndex() {
//            return _dataIndex;
//        }
//
//        // Multiple-line message functions
//        MultiLineMessage::MultiLineMessage() {
//            _dataIndex = LoggingSystem::GetLoggingHubInstance()->ConstructMessageStorage();
//        }
//
//        MultiLineMessage::MultiLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            _dataIndex = LoggingSystem::GetLoggingHubInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        void MultiLineMessage::Supply(LogMessageSeverity messageSeverity, const char *formatString, ...) const {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        MultiLineMessage::~MultiLineMessage() = default;
//
//        std::ostream &operator<<(std::ostream &stream, const MultiLineMessage &msg) {
//            LogMessageBackEndHeader& storage = LoggingSystem::GetLoggingHubInstance()->GetBackEnd(msg._dataIndex);
//            stream << *(storage._data);
//            storage._inUse = false;
//            storage._data->Clear();
//            return stream;
//        }
//
//        unsigned MultiLineMessage::GetDataIndex() {
//            return _dataIndex;
//        }
//    }
//}
//
//#endif
