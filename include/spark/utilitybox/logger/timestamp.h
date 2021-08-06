
#ifndef SPARK_TIMESTAMP_H
#define SPARK_TIMESTAMP_H

#include "spark/core/utility.h"

namespace Spark {
    namespace UtilityBox {

        class TimeStamp {
            public:
                typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;

                static void Init();

                TimeStamp();
                TimeStamp(const timepoint& time);
                ~TimeStamp();

                NODISCARD unsigned GetMillis() const;
                NODISCARD unsigned GetSeconds() const;
                NODISCARD unsigned GetMinutes() const;

            private:
                void Calculate();

                static timepoint initTime_;
                timepoint raw_;
                unsigned milliseconds_;
                unsigned seconds_;
                unsigned minutes_;
        };

    }
}

#endif // SPARK_TIMESTAMP_H
