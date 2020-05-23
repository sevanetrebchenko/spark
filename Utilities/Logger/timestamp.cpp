#include "timestamp.h"
#include "logger.h"

namespace UtilityBox::Timing {
    struct TimeStamp::TimeStampData {
        explicit TimeStampData(std::chrono::time_point<std::chrono::high_resolution_clock> timestamp);
        std::chrono::time_point<std::chrono::high_resolution_clock> _raw;
        unsigned _milliseconds;
        unsigned _seconds;
        unsigned _minutes;
    };

    TimeStamp::TimeStampData::TimeStampData(std::chrono::time_point<std::chrono::high_resolution_clock> timestamp) {
        // get the time in milliseconds
        _raw = timestamp;
        unsigned long elapsed = (_raw - Logger::GetLoggingInitializationTimestamp()).count() / 1000;
        _milliseconds = elapsed % 1000;
        _seconds = (elapsed / 1000) % 60;
        _minutes = (elapsed / 60000) % 60;
    }

    TimeStamp::TimeStamp() : _data(std::make_unique<TimeStampData>(std::chrono::high_resolution_clock::now())) {
    }

    TimeStamp::TimeStamp(const std::chrono::time_point<std::chrono::high_resolution_clock> &timestamp) : _data(std::make_unique<TimeStampData>(timestamp)) {
    }

    bool TimeStamp::operator<(const TimeStamp &other) const {
        return ConvertToMillis() < other.ConvertToMillis();
    }

    bool TimeStamp::operator==(const TimeStamp &other) const {
        return ConvertToMillis() == other.ConvertToMillis();
    }

    unsigned TimeStamp::ConvertToMillis() const {
        return _data->_milliseconds + (_data->_seconds * 1000) + (_data->_minutes * 60000);
    }

    unsigned TimeStamp::GetMillis() const {
        return _data->_milliseconds;
    }

    unsigned TimeStamp::GetSeconds() const {
        return _data->_seconds;
    }

    unsigned TimeStamp::GetMinutes() const {
        return _data->_minutes;
    }

    TimeStamp::TimeStamp(const TimeStamp &other) {
        _data = std::make_unique<TimeStampData>(other._data->_raw);
        _data->_milliseconds = other.GetMillis();
        _data->_seconds = other.GetSeconds();
        _data->_minutes = other.GetMinutes();
    }

    TimeStamp::~TimeStamp() {
        _data.reset();
    }

    TimeStamp::TimeStamp(TimeStamp &&other) noexcept {
        _data = std::move(other._data);
    }

    std::ostream &operator<<(std::ostream &os, const TimeStamp &stamp) {
        static std::stringstream format;

        // default format: 000m 00s 0000ms
        // append minutes
        format << std::setfill('0') << std::setw(3);
        format << stamp._data->_minutes << "m ";

        // append seconds
        format << std::setfill('0') << std::setw(2);
        format << stamp._data->_seconds << "s ";

        // append milliseconds
        format << std::setfill('0') << std::setw(4);
        format << stamp._data->_milliseconds << "ms ";

        os << format.str();
        format.str(std::string());
        return os;
    }
}
