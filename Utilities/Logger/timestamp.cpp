//
// Created by seva on 5/19/20.
//

#include "timestamp.h"

namespace UtilityBox::TimeStamp {
    struct TimeStamp::TimeStampData {
        TimeStampData() = default;
        std::chrono::time_point<std::chrono::high_resolution_clock> _raw;
        unsigned _milliseconds;
        unsigned _seconds;
        unsigned _minutes;
    };

    TimeStamp::TimeStamp() {
        _data = std::make_unique<TimeStampData>();
        // get the time in milliseconds
        unsigned long elapsed = 0;//(std::chrono::high_resolution_clock::now() - LoggingSystem::GetLoggingHubInstance()->GetLogStartTime()).count() / 1000;
        _data->_milliseconds = elapsed % 1000;
        _data->_seconds = (elapsed / 1000) % 60;
        _data->_minutes = (elapsed / 60000) % 60;
    }

    TimeStamp::TimeStamp(const std::chrono::time_point<std::chrono::high_resolution_clock> &timestamp) {
        _data = std::make_unique<TimeStampData>();
        // get the time in milliseconds
        unsigned long elapsed = 0;//(timestamp - LoggingSystem::GetLoggingHubInstance()->GetLogStartTime()).count() / 1000;
        _data->_milliseconds = elapsed % 1000;
        _data->_seconds = (elapsed / 1000) % 60;
        _data->_minutes = (elapsed / 60000) % 60;
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
        _data = std::make_unique<TimeStampData>();
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


}
