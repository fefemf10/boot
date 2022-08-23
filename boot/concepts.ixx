export module concepts;
import typetraits;
export namespace std
{
    template <class T1, class T2>
    concept same_as = is_same_v<T1, T2> && is_same_v<T2, T1>;

    template <class Derived, class Base>
    concept derived_from = __is_base_of(Base, Derived) && __is_convertible_to(const volatile Derived*, const volatile Base*);

    template <class From, class To>
    concept convertible_to = is_convertible_v<From, To> && requires {static_cast<To>(std::declval<From>());};

    template <class T> concept integral = is_integral_v<T>;
    template <class T> concept signed_integral = is_signed_v<T>;
    template <class T> concept unsigned_integral = is_unsigned_v<T>;
    template <class T> concept floating_point = is_floating_point_v<T>;

    template <class T>
    concept destructible = __is_nothrow_destructible(T);

    template <class T, class... _ArgTys>
    concept constructible_from = destructible<T> && __is_constructible(T, _ArgTys...);

    template <class T>
    concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

    template <class T>
    concept copy_constructible = move_constructible<T>
        && constructible_from<T, T&> && convertible_to<T&, T>
        && constructible_from<T, const T&> && convertible_to<const T&, T>
        && constructible_from<T, const T> && convertible_to<const T, T>;
}