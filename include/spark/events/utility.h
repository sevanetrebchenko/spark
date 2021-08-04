
#ifndef SPARK_EVENTS_UTILITY_H
#define SPARK_EVENTS_UTILITY_H

#include <spark/core/utility.h>

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

    }
}

//----------------------------------------------------------------------------------------------------------------------
// MACRO FOR DEFINING PURE VIRTUAL OnEvent FUNCTION FOR DERIVED CLASSES TO IMPLEMENT.
//----------------------------------------------------------------------------------------------------------------------
#define REGISTER_ON_EVENT(EVENT_TYPES...) Internal::RequireUniqueTypes<EVENT_TYPES>, public Internal::RequireOnEventForTypes<EVENT_TYPES>


//----------------------------------------------------------------------------------------------------------------------
// MACROS FOR REGISTERING CLASSES TO RECEIVE EVENTS OF GIVEN TYPES.
//----------------------------------------------------------------------------------------------------------------------
#define WRAP(TARGET, ...) Spark::Internal::Wrapper<TARGET, ##__VA_ARGS__>
#define REGISTER_EVENTS(BASE_CLASS, ...) Spark::Internal::Convert<BASE_CLASS, Spark::Events::IEventReceivable, Spark::Internal::Unwrap<__VA_ARGS__>>


//----------------------------------------------------------------------------------------------------------------------
// MACROS FOR REGISTERING OnEvent OVERRIDES FOR COMPONENT TYPES.
//----------------------------------------------------------------------------------------------------------------------
#include <spark/core/macro_tables.h>

#define EXPAND_MACRO(macro, n, types...) EXPAND##n(macro, ##types)
#define REGISTER_COMPONENT_HELPER(macro, n, types...) EXPAND_MACRO(macro, n, types)
#define COMPONENT_ON_EVENT_REGISTRATION(ComponentType) void OnEvent(Spark::Events::AddComponentEvent<ComponentType>* event) override {                                                                                                                                         \
                                                           static_assert(std::is_base_of<Spark::ECS::BaseComponent, ComponentType>::value, "OnEvent function parameter must take an AddComponentEvent instance with template parameter deriving from ECS::BaseComponent.");    \
                                                           AddComponent<ComponentType>(event->GetEntityID());                                                                                                                                                                  \
                                                       }                                                                                                                                                                                                                       \
                                                       void OnEvent(Spark::Events::RemoveComponentEvent<ComponentType>* event) override {                                                                                                                                      \
                                                           static_assert(std::is_base_of<Spark::ECS::BaseComponent, ComponentType>::value, "OnEvent function parameter must take an RemoveComponentEvent instance with template parameter deriving from ECS::BaseComponent."); \
                                                           RemoveComponent<ComponentType>(event->GetEntityID());                                                                                                                                                               \
                                                       }

// Generates two OnEvent overrides (AddComponentEvent, RemoveComponentEvent) for each component type passed in. Component types must derive from ECS::BaseComponent.
// For function declarations, see COMPONENT_REGISTRATION above.
// Used in ecs/entity_manager.h
#define REGISTER_COMPONENT_TYPE_OVERRIDES(ComponentTypes...) REGISTER_COMPONENT_HELPER(COMPONENT_ON_EVENT_REGISTRATION, GET_NUM_ARGS_ELEMENT_FROM_TABLE(ARG_NUMBERS, ComponentTypes), ComponentTypes)

#endif //SPARK_EVENTS_UTILITY_H
