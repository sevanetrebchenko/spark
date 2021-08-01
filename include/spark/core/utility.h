
#ifndef SPARK_UTILITY_H
#define SPARK_UTILITY_H

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

#endif //SPARK_UTILITY_H
