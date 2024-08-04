export module sl.pair;
import sl.typetraits;
import sl.utility;
export namespace std
{
	template <class T1, class T2>
	struct pair
	{
		using first_type = T1;
		using second_type = T2;

		template <class _Uty1 = T1, class _Uty2 = T2, enable_if_t<conjunction_v<is_default_constructible<_Uty1>, is_default_constructible<_Uty2>>, int> = 0>
		constexpr explicit(!conjunction_v<_Is_implicitly_default_constructible<_Uty1>, _Is_implicitly_default_constructible<_Uty2>>) pair() noexcept( is_nothrow_default_constructible_v<_Uty1>&& is_nothrow_default_constructible_v<_Uty2>) : first(), second() {}

		template <class _Uty1 = T1, class _Uty2 = T2, enable_if_t<conjunction_v<is_copy_constructible<_Uty1>, is_copy_constructible<_Uty2>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<const _Uty1&, _Uty1>, is_convertible<const _Uty2&, _Uty2>>) pair(const T1& _Val1, const T2& _Val2) noexcept(is_nothrow_copy_constructible_v<_Uty1>&& is_nothrow_copy_constructible_v<_Uty2>) : first(_Val1), second(_Val2) {}

		template <class _Other1 = T1, class _Other2 = T2, enable_if_t<conjunction_v<is_constructible<T1, _Other1>, is_constructible<T2, _Other2>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<_Other1, T1>, is_convertible<_Other2, T2>>) pair(_Other1&& _Val1, _Other2&& _Val2) noexcept(is_nothrow_constructible_v<T1, _Other1>&& is_nothrow_constructible_v<T2, _Other2>) : first(forward<_Other1>(_Val1)), second(forward<_Other2>(_Val2)) {}

		pair(const pair&) = default;
		pair(pair&&) = default;

		template <class _Other1, class _Other2, enable_if_t<conjunction_v<is_constructible<T1, _Other1&>, is_constructible<T2, _Other2&>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<_Other1&, T1>, is_convertible<_Other2&, T2>>) pair(pair<_Other1, _Other2>& right) noexcept(is_nothrow_constructible_v<T1, _Other1&>&& is_nothrow_constructible_v<T2, _Other2&>) : first(right.first), second(right.second) {}

		template <class _Other1, class _Other2, enable_if_t<conjunction_v<is_constructible<T1, const _Other1&>, is_constructible<T2, const _Other2&>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<const _Other1&, T1>, is_convertible<const _Other2&, T2>>) pair(const pair<_Other1, _Other2>& right) noexcept(is_nothrow_constructible_v<T1, const _Other1&>&& is_nothrow_constructible_v<T2, const _Other2&>) : first(right.first), second(right.second) {}

		template <class _Other1, class _Other2, enable_if_t<conjunction_v<is_constructible<T1, _Other1>, is_constructible<T2, _Other2>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<_Other1, T1>, is_convertible<_Other2, T2>>) pair(pair<_Other1, _Other2>&& right) noexcept(is_nothrow_constructible_v<T1, _Other1>&& is_nothrow_constructible_v<T2, _Other2>) : first(forward<_Other1>(right.first)), second(forward<_Other2>(right.second)) {}

		template <class _Other1, class _Other2, enable_if_t<conjunction_v<is_constructible<T1, const _Other1>, is_constructible<T2, const _Other2>>, int> = 0>
		constexpr explicit(!conjunction_v<is_convertible<const _Other1, T1>, is_convertible<const _Other2, T2>>) pair(const pair<_Other1, _Other2>&& right) noexcept(is_nothrow_constructible_v<T1, const _Other1>&& is_nothrow_constructible_v<T2, const _Other2>) : first(forward<const _Other1>(right.first)), second(forward<const _Other2>(right.second)) {}

		template <_Pair_like _Other> requires conjunction_v<bool_constant<!_Is_subrange_v<remove_cvref_t<_Other>>>, is_constructible<T1, decltype(get<0>(declval<_Other>()))>, is_constructible<T2, decltype(get<1>(declval<_Other>()))>>
		constexpr explicit(!conjunction_v<is_convertible<decltype(get<0>(declval<_Other>())), T1>, is_convertible<decltype(get<1>(declval<_Other>())), T2>>) pair(_Other&& right) noexcept(is_nothrow_constructible_v<T1, decltype(get<0>(declval<_Other>()))>&& is_nothrow_constructible_v<T2, decltype(get<1>(declval<_Other>()))>) : first(get<0>(forward<_Other>(right))), second(get<1>(forward<_Other>(right))) {}

		template <class _Tuple1, class _Tuple2, size_t... _Indices1, size_t... _Indices2>
		constexpr pair(_Tuple1& _Val1, _Tuple2& _Val2, index_sequence<_Indices1...>, index_sequence<_Indices2...>) : first(_Tuple_get<_Indices1>(move(_Val1))...), second(_Tuple_get<_Indices2>(move(_Val2))...) {}

		template <class... _Types1, class... _Types2>
		constexpr pair(piecewise_construct_t, tuple<_Types1...> _Val1, tuple<_Types2...> _Val2) : pair(_Val1, _Val2, index_sequence_for<_Types1...>{}, index_sequence_for<_Types2...>{}) {}

		pair& operator=(const volatile pair&) = delete;

		template <class _Myself = pair, enable_if_t<conjunction_v<_Is_copy_assignable_no_precondition_check<typename _Myself::first_type>, _Is_copy_assignable_no_precondition_check<typename _Myself::second_type>>, int> = 0>
		constexpr pair& operator=(_Identity_t<const _Myself&> right) noexcept(conjunction_v<is_nothrow_copy_assignable<T1>, is_nothrow_copy_assignable<T2>>)
		{
			first = right.first;
			second = right.second;
			return *this;
		}

		template <class _Myself = pair,
			enable_if_t<conjunction_v<_Is_copy_assignable_no_precondition_check<const typename _Myself::first_type>,
			_Is_copy_assignable_no_precondition_check<const typename _Myself::second_type>>,
			int> = 0>
			constexpr const pair& operator=(_Identity_t<const _Myself&> right) const
			noexcept(conjunction_v<is_nothrow_copy_assignable<const T1>,
				is_nothrow_copy_assignable<const T2>>) /* strengthened */ {
			first = right.first;
			second = right.second;
			return *this;
		}

		template <class _Myself = pair,
			enable_if_t<conjunction_v<_Is_move_assignable_no_precondition_check<typename _Myself::first_type>,
			_Is_move_assignable_no_precondition_check<typename _Myself::second_type>>,
			int> = 0>
			constexpr pair& operator=(_Identity_t<_Myself&&> right) noexcept(
				conjunction_v<is_nothrow_move_assignable<T1>, is_nothrow_move_assignable<T2>>) /* strengthened */ {
			first = forward<T1>(right.first);
			second = forward<T2>(right.second);
			return *this;
		}

		template <class _Myself = pair,
			enable_if_t<conjunction_v<_Is_assignable_no_precondition_check<const typename _Myself::first_type&, T1>,
			_Is_assignable_no_precondition_check<const typename _Myself::second_type&, T2>>,
			int> = 0>
			constexpr const pair& operator=(_Identity_t<_Myself&&> right) const
			noexcept(conjunction_v<is_nothrow_assignable<const T1&, T1>,
				is_nothrow_assignable<const T2&, T2>>) /* strengthened */ {
			first = forward<T1>(right.first);
			second = forward<T2>(right.second);
			return *this;
		}

		template <class _Other1, class _Other2,
			enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<T1&, const _Other1&>,
			is_assignable<T2&, const _Other2&>>,
			int> = 0>
			constexpr pair& operator=(const pair<_Other1, _Other2>& right) noexcept(
				is_nothrow_assignable_v<T1&, const _Other1&>&&
				is_nothrow_assignable_v<T2&, const _Other2&>) /* strengthened */ {
			first = right.first;
			second = right.second;
			return *this;
		}

		template <class _Other1, class _Other2,
			enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>,
			is_assignable<const T1&, const _Other1&>, is_assignable<const T2&, const _Other2&>>,
			int> = 0>
			constexpr const pair& operator=(const pair<_Other1, _Other2>& right) const
			noexcept(is_nothrow_assignable_v<const T1&, const _Other1&>&&
				is_nothrow_assignable_v<const T2&, const _Other2&>) /* strengthened */ {
			first = right.first;
			second = right.second;
			return *this;
		}

		template <class _Other1, class _Other2,
			enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<T1&, _Other1>,
			is_assignable<T2&, _Other2>>,
			int> = 0>
			constexpr pair& operator=(pair<_Other1, _Other2>&& right) noexcept(
				is_nothrow_assignable_v<T1&, _Other1>&& is_nothrow_assignable_v<T2&, _Other2>) /* strengthened */ {
			first = forward<_Other1>(right.first);
			second = forward<_Other2>(right.second);
			return *this;
		}

		template <class _Other1, class _Other2,
			enable_if_t<conjunction_v<negation<is_same<pair, pair<_Other1, _Other2>>>, is_assignable<const T1&, _Other1>,
			is_assignable<const T2&, _Other2>>,
			int> = 0>
			constexpr const pair& operator=(pair<_Other1, _Other2>&& right) const
			noexcept(is_nothrow_assignable_v<const T1&, _Other1>&&
				is_nothrow_assignable_v<const T2&, _Other2>) /* strengthened */ {
			first = forward<_Other1>(right.first);
			second = forward<_Other2>(right.second);
			return *this;
		}

		template <_Pair_like _Other>
			requires _Different_from<_Other, pair> && (!_Is_subrange_v<remove_cvref_t<_Other>>)
		&& is_assignable_v<T1&, decltype(get<0>(declval<_Other>()))>
			&& is_assignable_v<T2&, decltype(get<1>(declval<_Other>()))>
			constexpr pair& operator=(_Other&& right) noexcept(
				is_nothrow_assignable_v<T1&, decltype(get<0>(declval<_Other>()))>&&
				is_nothrow_assignable_v<T2&, decltype(get<1>(declval<_Other>()))>)
		{
			first = get<0>(forward<_Other>(right));
			second = get<1>(forward<_Other>(right));
			return *this;
		}

		template <_Pair_like _Other>
			requires _Different_from<_Other, pair> && (!_Is_subrange_v<remove_cvref_t<_Other>>)
		&& is_assignable_v<const T1&, decltype(get<0>(declval<_Other>()))>
			&& is_assignable_v<const T2&, decltype(get<1>(declval<_Other>()))>
			constexpr const pair& operator=(_Other&& right) const noexcept(
				is_nothrow_assignable_v<const T1&, decltype(get<0>(declval<_Other>()))>&&
				is_nothrow_assignable_v<const T2&, decltype(get<1>(declval<_Other>()))>) /* strengthened */ {
			first = get<0>(forward<_Other>(right));
			second = get<1>(forward<_Other>(right));
			return *this;
		}

		constexpr void swap(pair& right) noexcept(_Is_nothrow_swappable<T1>::value&& _Is_nothrow_swappable<T2>::value)
		{
			if (this != addressof(right))
			{
				swap(first, right.first);
				swap(second, right.second);
			}
		}


		template <int = 0>
		constexpr void swap(const pair& right) const noexcept(is_nothrow_swappable_v<const T1>&& is_nothrow_swappable_v<const T2>)
		{
			if (this != addressof(right))
			{
				swap(first, right.first);
				swap(second, right.second);
			}
		}

		T1 first;
		T2 second;
	};
}