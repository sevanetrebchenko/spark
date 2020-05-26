
#include "adapter.h"
#include "../exception.h"
#include <utility>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace UtilityBox {
    namespace Logger {
        class Adapter::AdapterData {
            public:
                AdapterData();
                ~AdapterData();
                const char* FormatCalendarBuffer(std::string formatString);
                const char* FormatLogCounterBuffer(const unsigned& logCounter);
                const char* FormatMessageSeverityBuffer(const LogMessageSeverity& messageSeverity);
                const char* FormatMessageTimestampBuffer(const Timing::TimeStamp& timestamp);

            private:
                void UpdateCalendarBuffer();

                std::stringstream _format;

                // calendar
                std::string _calendarFormat;
                std::time_t _calendarTime;
                unsigned _calendarBufferSize;
                char* _calendarBuffer;

                // log counter
                unsigned _logCounterBufferSize;
                char* _logCounterBuffer;

                // message severity
                unsigned _messageSeverityBufferSize;
                char* _messageSeverityBuffer;

                // timestamp
                unsigned _timestampBufferSize;
                char* _timestampBuffer;
        };

        Adapter::AdapterData::AdapterData() : _calendarTime(std::time(nullptr)), _calendarBufferSize(64u),
                                                                                       _logCounterBufferSize(16u),
                                                                                       _messageSeverityBufferSize(16u),
                                                                                       _timestampBufferSize(16u) {
            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
            _logCounterBuffer = new(std::nothrow) char[_logCounterBufferSize];
            _messageSeverityBuffer = new(std::nothrow) char[_messageSeverityBufferSize];
            _timestampBuffer = new(std::nothrow) char[_timestampBufferSize];
            // todo: check
        }

        void Adapter::AdapterData::UpdateCalendarBuffer() {
            while(!std::strftime(_calendarBuffer, _calendarBufferSize, _calendarFormat.c_str(), std::localtime(&_calendarTime))) {
                _calendarBufferSize *= 2;
                delete[] _calendarBuffer;
                _calendarBuffer = new (std::nothrow) char[_calendarBufferSize];
                // todo: check
            }
        }

        Adapter::AdapterData::~AdapterData() {
            delete[] _calendarBuffer;
            delete[] _logCounterBuffer;
            delete[] _messageSeverityBuffer;
            delete[] _timestampBuffer;
        }

        const char *Adapter::AdapterData::FormatCalendarBuffer(std::string formatString) {
            if (_calendarFormat != formatString) {
                _calendarFormat = std::move(formatString);
                UpdateCalendarBuffer();
            }

            return _calendarBuffer;
        }

        const char *Adapter::AdapterData::FormatLogCounterBuffer(const unsigned &logCounter) {
            _format << '[';
            _format << std::setfill('0') << std::setw(4);
            _format << logCounter / 1000;

            _format << ' ';

            _format << std::setfill('0') << std::setw(4);
            _format << logCounter % 1000;
            _format << ']';

            strcpy(_logCounterBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _logCounterBuffer;
        }

        const char *Adapter::AdapterData::FormatMessageSeverityBuffer(const LogMessageSeverity &messageSeverity) {
            _format << '[';

            // [ DEBUG ]
            // [ WARNING ]
            // [ SEVERE ]
            // [ UNKNOWN ]
            switch (messageSeverity) {
                case LogMessageSeverity::DEBUG:
                    _format << " DEBUG ";
                    break;
                case LogMessageSeverity::WARNING:
                    _format << " WARNING ";
                    break;
                case LogMessageSeverity::SEVERE:
                    _format << " SEVERE ";
                    break;
                default:
                    _format << " UNKNOWN ";
                    break;
            }

            _format << ']';

            strcpy(_messageSeverityBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _messageSeverityBuffer;
        }

        const char *Adapter::AdapterData::FormatMessageTimestampBuffer(const Timing::TimeStamp &timestamp) {
            // timestamp format: [000m 00s 0000ms]
            _format << '[';
            // minutes
            _format << std::setfill('0') << std::setw(3);
            _format << timestamp.GetMinutes();
            _format << 'm';

            _format << ' ';

            // seconds
            _format << std::setfill('0') << std::setw(2);
            _format << timestamp.GetSeconds();
            _format << 's';

            _format << ' ';

            // milliseconds
            _format << std::setfill('0') << std::setw(4);
            _format << timestamp.GetMillis();
            _format << "ms";

            _format << ']';

            strcpy(_timestampBuffer, _format.str().c_str());

            // clear buffer
            _format.str(std::string());

            return _timestampBuffer;
        }

        Adapter::Adapter(std::string &&name) : _adapterName(std::move(name)), _logCount(0), _config(), _data(std::move(std::make_unique<AdapterData>())) {
        }

        AdapterConfiguration &Adapter::GetConfiguration() {
            return _config;
        }

        const char *Adapter::FormatCalendarInformation() {
            return _data->FormatCalendarBuffer(_config.calendarFormat);
        }

        const char *Adapter::FormatLogCounter() {
            return _data->FormatLogCounterBuffer(_logCount);
        }

        const char *Adapter::FormatSeverity(LogMessageSeverity messageSeverity) {
            return _data->FormatMessageSeverityBuffer(messageSeverity);
        }

        const char *Adapter::FormatTimestamp(const Timing::TimeStamp &timeStamp) {
            return _data->FormatMessageTimestampBuffer(timeStamp);
        }

        Adapter::~Adapter() {
            _data.reset();
        }
    }
}
