export module std:concepts;
import :typetraits;
export namespace std
{
    template <class T1, class T2>
    concept same_as = is_same_v<T1, T2> && is_same_v<T2, T1>;

    template <class Derived, class Base>
    concept derived_from = __is_base_of(Base, Derived) && __is_convertible_to(const volatile Derived*, const volatile Base*);

    template <class From, class To>
    concept convertible_to = is_convertible_v<From, To> && requires {static_cast<To>(declval<From>());};

    template <class From, class To>
    concept _Implicitly_convertible_to = is_convertible_v<From, To>;

    template <class T1, class T2>
    concept common_reference_with =
        requires {
        typename common_reference_t<T1, T2>;
        typename common_reference_t<T2, T1>;
    }
    && same_as<common_reference_t<T1, T2>, common_reference_t<T2, T1>>
        && convertible_to<T1, common_reference_t<T1, T2>>
        && convertible_to<T2, common_reference_t<T1, T2>>;

    template <class T1, class T2>
    concept common_with =
        requires {
        typename common_type_t<T1, T2>;
        typename common_type_t<T2, T1>;
    }
    && same_as<common_type_t<T1, T2>, common_type_t<T2, T1>>
        && requires {
        static_cast<common_type_t<T1, T2>>(declval<T1>());
        static_cast<common_type_t<T1, T2>>(declval<T2>());
    }
    && common_reference_with<add_lvalue_reference_t<const T1>, add_lvalue_reference_t<const T2>>
        && common_reference_with<add_lvalue_reference_t<common_type_t<T1, T2>>,
        common_reference_t<add_lvalue_reference_t<const T1>, add_lvalue_reference_t<const T2>>>;

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

    template <class T>
    concept _Boolean_testable_impl = convertible_to<T, bool>;

    template <class T>
    concept _Boolean_testable = _Boolean_testable_impl<T>
        && requires(T && __t) {
            { !static_cast<T&&>(__t) } -> _Boolean_testable_impl;
    };

    template <class T1, class T2>
    concept _Half_equality_comparable =
        requires(const remove_reference_t<T1>&__x, const remove_reference_t<T2>&__y) {
            { __x == __y } -> _Boolean_testable;
            { __x != __y } -> _Boolean_testable;
    };

    template <class T1, class T2>
    concept _Weakly_equality_comparable_with =
        _Half_equality_comparable<T1, T2> && _Half_equality_comparable<T2, T1>;

    template <class T>
    concept equality_comparable = _Half_equality_comparable<T, T>;

    template <class T1, class T2>
    concept equality_comparable_with = equality_comparable<T1> && equality_comparable<T2>
        && common_reference_with<const remove_reference_t<T1>&, const remove_reference_t<T2>&>
        && equality_comparable<common_reference_t<const remove_reference_t<T1>&, const remove_reference_t<T2>&>>
        && _Weakly_equality_comparable_with<T1, T2>;

    template <class T1, class T2>
    concept _Half_ordered = requires(const remove_reference_t<T1>&__t, const remove_reference_t<T2>&__u) {
        { __t < __u } -> _Boolean_testable;
        { __t > __u } -> _Boolean_testable;
        { __t <= __u } -> _Boolean_testable;
        { __t >= __u } -> _Boolean_testable;
    };

    template <class T1, class T2>
    concept _Partially_ordered_with = _Half_ordered<T1, T2> && _Half_ordered<T2, T1>;

    template <class T>
    concept totally_ordered = equality_comparable<T> && _Half_ordered<T, T>;

    template <class T1, class T2>
    concept totally_ordered_with = totally_ordered<T1> && totally_ordered<T2>
        && equality_comparable_with<T1, T2>
        && totally_ordered<common_reference_t<const remove_reference_t<T1>&, const remove_reference_t<T2>&>>
        && _Partially_ordered_with<T1, T2>;

    template <class _Ty>
    struct _Choice_t {
        _Ty _Strategy = _Ty{};
        bool _No_throw = false;
    };
}