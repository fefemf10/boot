export module sl.concepts;
import sl.typetraits;
export namespace std
{
	template <class T1, class T2>
	concept _Same_impl = is_same_v<T1, T2>;

	template <class T1, class T2>
	concept same_as = is_same_v<T1, T2>&& is_same_v<T2, T1>;

	template <class Derived, class Base>
	concept derived_from = __is_base_of(Base, Derived) && __is_convertible_to(const volatile Derived*, const volatile Base*);

	template <class From, class To>
	concept convertible_to = is_convertible_v<From, To>&& requires {static_cast<To>(declval<From>()); };

	template <class T> concept cis_union =  __is_union(T);

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
	template <class T> concept character = is_character_v<T>;

	template <class T1, class T2>
	concept assignable_from = is_lvalue_reference_v<T1>
		&& common_reference_with<const remove_reference_t<T1>&, const remove_reference_t<T2>&>
		&& requires(T1 _Left, T2&& _Right) {
			{ _Left = static_cast<T2&&>(_Right) } -> same_as<T1>;
	};

	template <class T>
	concept destructible = __is_nothrow_destructible(T);

	template <class T, class... _ArgTys>
	concept constructible_from = destructible<T> && __is_constructible(T, _ArgTys...);

	template <class T>
	concept default_initializable = constructible_from<T>
		&& requires {
		T{};
		::new (static_cast<void*>(nullptr)) T;
	};

	template <class T>
	concept move_constructible = constructible_from<T, T>&& convertible_to<T, T>;

	template <class T>
	concept _Has_class_or_enum_type = __is_class(remove_reference_t<T>) || __is_enum(remove_reference_t<T>) || __is_union(remove_reference_t<T>);

	namespace ranges {
		namespace _Swap {
			template <class T>
			void swap(T&, T&) = delete;

			template <class T1, class T2>
			concept _Use_ADL_swap = (_Has_class_or_enum_type<T1> || _Has_class_or_enum_type<T2>)
				&& requires(T1 && __t, T2 && __u) {
				swap(static_cast<T1&&>(__t), static_cast<T2&&>(__u)); // intentional ADL
			};

			struct _Cpo {
				template <class T1, class T2>
					requires _Use_ADL_swap<T1, T2>
				constexpr void operator()(T1&& __t, T2&& __u) const
					noexcept(noexcept(swap(static_cast<T1&&>(__t), static_cast<T2&&>(__u)))) { // intentional ADL
					swap(static_cast<T1&&>(__t), static_cast<T2&&>(__u)); // intentional ADL
				}

				template <class T>
					requires (!_Use_ADL_swap<T&, T&>&& move_constructible<T>&& assignable_from<T&, T>)
				constexpr void operator()(T& __x, T& __y) const
					noexcept(is_nothrow_move_constructible_v<T>&& is_nothrow_move_assignable_v<T>) {
					T __tmp(static_cast<T&&>(__x));
					__x = static_cast<T&&>(__y);
					__y = static_cast<T&&>(__tmp);
				}

				template <class T1, class T2, size_t _Size>
				constexpr void operator()(T1(&__t)[_Size], T2(&__u)[_Size]) const
					noexcept(noexcept((*this)(__t[0], __u[0])))
					requires requires { (*this)(__t[0], __u[0]); } {
					for (size_t __i = 0; __i < _Size; ++__i) {
						(*this)(__t[__i], __u[__i]);
					}
				}
			};
		}

		inline namespace _Cpos {
			inline constexpr _Swap::_Cpo swap;
		}
	}

	template <class T>
	concept swappable = requires(T & __x, T & __y) {
		ranges::swap(__x, __y);
	};

	template <class T1, class T2>
	concept swappable_with = common_reference_with<T1, T2>
		&& requires(T1&& __t, T2&& __u) {
		ranges::swap(static_cast<T1&&>(__t), static_cast<T1&&>(__t));
		ranges::swap(static_cast<T2&&>(__u), static_cast<T2&&>(__u));
		ranges::swap(static_cast<T1&&>(__t), static_cast<T2&&>(__u));
		ranges::swap(static_cast<T2&&>(__u), static_cast<T1&&>(__t));
	};

	template <class T>
	concept copy_constructible = move_constructible<T>
		&& constructible_from<T, T&>&& convertible_to<T&, T>
		&& constructible_from<T, const T&>&& convertible_to<const T&, T>
		&& constructible_from<T, const T>&& convertible_to<const T, T>;

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
		_Half_equality_comparable<T1, T2>&& _Half_equality_comparable<T2, T1>;

	template <class T>
	concept equality_comparable = _Half_equality_comparable<T, T>;


	template <class T1, class T2, class _Ref = common_reference_t<const T1&, const T2&>>
	concept _Comparison_common_type_with_impl =
		same_as<_Ref, common_reference_t<const T2&, const T1&>>
		&& requires {
		requires convertible_to<const T1&, const _Ref&> || convertible_to<T1, const _Ref&>;
		requires convertible_to<const T2&, const _Ref&> || convertible_to<T2, const _Ref&>;
	};

	template <class T1, class T2>
	concept _Comparison_common_type_with = _Comparison_common_type_with_impl<remove_cvref_t<T1>, remove_cvref_t<T2>>;

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
	concept _Partially_ordered_with = _Half_ordered<T1, T2>&& _Half_ordered<T2, T1>;

	template <class T>
	concept totally_ordered = equality_comparable<T> && _Half_ordered<T, T>;

	template <class T1, class T2>
	concept totally_ordered_with = totally_ordered<T1> && totally_ordered<T2>
		&& equality_comparable_with<T1, T2>
		&& totally_ordered<common_reference_t<const remove_reference_t<T1>&, const remove_reference_t<T2>&>>
		&& _Partially_ordered_with<T1, T2>;

	template <class T>
	concept movable = is_object_v<T>
		&& move_constructible<T>
		&& assignable_from<T&, T>
		&& swappable<T>;

	template <class T>
	concept copyable = copy_constructible<T>
		&& movable<T>
		&& assignable_from<T&, T&>
		&& assignable_from<T&, const T&>
		&& assignable_from<T&, const T>;

	template <class T>
	concept semiregular = copyable<T> && default_initializable<T>;

	template <class T>
	concept regular = semiregular<T> && equality_comparable<T>;

	template <class _FTy, class... _ArgTys>
	concept invocable = requires(_FTy && _Fn, _ArgTys&&... _Args) {
		invoke(static_cast<_FTy&&>(_Fn), static_cast<_ArgTys&&>(_Args)...);
	};

	template <class _FTy, class... _ArgTys>
	concept regular_invocable = invocable<_FTy, _ArgTys...>;

	template <class _FTy, class... _ArgTys>
	concept predicate = regular_invocable<_FTy, _ArgTys...>
		&& _Boolean_testable<invoke_result_t<_FTy, _ArgTys...>>;

	template <class _FTy, class T1, class T2>
	concept relation =
		predicate<_FTy, T1, T1>
		&& predicate<_FTy, T2, T2>
		&& predicate<_FTy, T1, T2>
		&& predicate<_FTy, T2, T1>;

	template <class _FTy, class T1, class T2>
	concept equivalence_relation = relation<_FTy, T1, T2>;

	template <class _FTy, class T1, class T2>
	concept strict_weak_order = relation<_FTy, T1, T2>;

	template <class T>
	struct _Choice_t {
		T _Strategy = T{};
		bool _No_throw = false;
	};
}