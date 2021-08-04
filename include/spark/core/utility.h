
#ifndef SPARK_CORE_UTILITY_H
#define SPARK_CORE_UTILITY_H

namespace Spark {
    namespace Internal {

        // Unique types.
        template <class... Types>
        struct RequireUniqueTypes {
            static_assert(sizeof...(Types) > 1, "Invalid number template arguments.");
        };

        template <typename T1, typename T2, typename... Types>
        struct RequireUniqueTypes<T1, T2, Types ...> : RequireUniqueTypes<T1, T2>, RequireUniqueTypes<T1, Types...>, RequireUniqueTypes<T2, Types...> {
        };

        template <typename T1, typename T2>
        struct RequireUniqueTypes<T1, T2> {
            static_assert(!std::is_same<T1, T2>::value, "Template parameters must be unique types.");
        };

    }
}

#include <spark/core/macro_tables.h>
#define NUM_ARGS(...) ARG_TABLE(__VA_ARGS__)
#define GET_NUM_ARGS_ELEMENT_FROM_TABLE(TABLE, ...) NUM_ARGS(_, ##__VA_ARGS__, TABLE())

#endif //SPARK_CORE_UTILITY_H
