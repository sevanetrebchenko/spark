
#ifndef SPARK_TIMESTAMP_H
#define SPARK_TIMESTAMP_H

#include <spark/core/rename.h>

namespace Spark {
    namespace UtilityBox {
        namespace Timing {

            class TimeStamp {
                public:
                    /**
                    * Constructor for a timestamp. Uses std::chrono::high_resolution_clock::now() (time when constructor was
                    * called) as the time record.
                    */
                    TimeStamp();

                    /**
                    * Timestamp move constructor.
                    * @param other - Timestamp to move data from.
                    */
                    TimeStamp(TimeStamp&& other) noexcept;

                    /**
                    * Timestamp destructor.
                    */
                    ~TimeStamp();

                    /**
                    * Get only the millisecond portion of this timestamp.
                    * @return Millisecond portion of this timestamp.
                    */
                    NODISCARD unsigned GetMillis() const;

                    /**
                    * Get only the second portion of this timestamp.
                    * @return Second portion of this timestamp.
                    */
                    NODISCARD unsigned GetSeconds() const;

                    /**
                    * Get only the minute portion of this timestamp.
                    * @return Minute portion of this timestamp.
                    */
                    NODISCARD unsigned GetMinutes() const;

                    /**
                    * Print this timestamp to the provided stream.
                    * @param os    - Stream to print to.
                    * @param stamp - Timestamp object to print.
                    * @return Printed-to stream.
                    */
                    friend std::ostream& operator<<(std::ostream& os, const TimeStamp& stamp);

                private:
                    // Storage for Timestamp data, back-end functionality, and helper functions.
                    struct TimeStampData;
                    TimeStampData* _data;
            };

        }
    }
}

#endif // SPARK_TIMESTAMP_H
