
#ifndef SPARK_GENERATORS_H
#define SPARK_GENERATORS_H

#include <spark/spark_pch.h>

#include <tuple>

namespace Spark {

    namespace Internal {

        // OnEvent.
        template <typename... EventTypes>
        struct RequireOnEventForTypes {
            static_assert(sizeof...(EventTypes) > 0, "Invalid number template arguments.");
        };

        template <typename EventType, typename... AdditionalEventTypes>
        struct RequireOnEventForTypes<EventType, AdditionalEventTypes...> : RequireOnEventForTypes<EventType>, RequireOnEventForTypes<AdditionalEventTypes...> {
            using RequireOnEventForTypes<EventType>::OnEvent;
        };

        template <typename EventType>
        struct RequireOnEventForTypes<EventType> {
            virtual void OnEvent(EventType* eventPointer) = 0;
        };

#define REGISTER_ON_EVENT(EVENT_TYPES...) Internal::RequireUniqueTypes<EVENT_TYPES>, public Internal::RequireOnEventForTypes<EVENT_TYPES>


        // Generator for IEventReceivable.
        // Converter from std::tuple<Types...> to Base<Types...>
        template <typename BaseClass, template <typename... Types> class Base, typename Tuple>
        struct ConvertImpl {
        };

        template <typename BaseClass, template <typename... Types> class Base, typename... Types>
        struct ConvertImpl<BaseClass, Base, std::tuple<Types...>> {
            using result = Base<BaseClass, typename std::remove_pointer<Types>::type...>; // CRTP for base class.
        };

        template <typename BaseClass, template <typename... Types> class Base, typename Tuple>
        using Convert = typename ConvertImpl<BaseClass, Base, Tuple>::result;

        // Wraps all types into a std::tuple of all types.
        template <template <typename T> class Target, typename... Types>
        struct Wrapper {
            template<typename U>
            using target = typename std::add_pointer<Target<U>>::type; // Use pointer to not impose default constructor requirement on events.
        };

        // Unwraps wrappers (tuples) of types into one tuple containing all wrapped types.
        // Unwrap for basic types.
        template <typename T>
        std::tuple<T> UnwrapImpl(T);

        // Unwrap for Wrapper.
        template <template <typename T> class Target, typename... Types>
        std::tuple<typename Wrapper<Target, Types...>::template target<Types>...> UnwrapImpl(Wrapper<Target, Types...>*);

        template <typename... Wrappers>
        using Unwrap = decltype(std::tuple_cat(decltype(UnwrapImpl((typename std::add_pointer<Wrappers>::type){})){}...)); // Create pointer to type to not impose default constructor requirement on events.

#define WRAP(TARGET, ...) Internal::Wrapper<TARGET, ##__VA_ARGS__>
#define REGISTER_EVENTS(BASE_CLASS, ...) Internal::Convert<BASE_CLASS, Events::IEventReceivable, Internal::Unwrap<__VA_ARGS__>>

    }

}

#endif //SPARK_GENERATORS_H
