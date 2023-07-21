export module sl.utility;
import sl.typetraits;
import sl.bit;
import sl.type;
import sl.concepts;
namespace std
{
	template <class T>
	struct _Get_first_parameter;

	template <template <class, class...> class T, class _First, class... _Rest>
	struct _Get_first_parameter<T<_First, _Rest...>>
	{
		using type = _First;
	};

	template <class _Newfirst, class T>
	struct _Replace_first_parameter;

	template <class _Newfirst, template <class, class...> class T, class _First, class... _Rest>
	struct _Replace_first_parameter<_Newfirst, T<_First, _Rest...>>
	{
		using type = T<_Newfirst, _Rest...>;
	};

	template <class T, class = void>
	struct _Get_element_type
	{
		using type = typename _Get_first_parameter<T>::type;
	};

	template <class T>
	struct _Get_element_type<T, void_t<typename T::element_type>>
	{
		using type = typename T::element_type;
	};

	template <class T, class = void>
	struct _Get_ptr_difference_type
	{
		using type = ptrdiff_t;
	};

	template <class T>
	struct _Get_ptr_difference_type<T, void_t<typename T::difference_type>>
	{
		using type = typename T::difference_type;
	};

	template <class T, class U, class = void>
	struct _Get_rebind_alias
	{
		using type = typename _Replace_first_parameter<U, T>::type;
	};

	template <class T, class U>
	struct _Get_rebind_alias<T, U, void_t<typename T::template rebind<U>>>
	{
		using type = typename T::template rebind<U>;
	};

	template <class T>
	using _With_reference = T&;

	template <class T>
	concept _Can_reference = requires
	{
		typename _With_reference<T>;
	};

	template <class T>
	concept _Dereferenceable = requires(T & __t)
	{
		{ *__t } -> _Can_reference;
	};

	template <class T>
	concept _Has_member_iterator_concept = requires
	{
		typename T::iterator_concept;
	};

	template <class T>
	concept _Has_member_iterator_category = requires
	{
		typename T::iterator_category;
	};

	template <class T>
	concept _Has_member_value_type = requires
	{
		typename T::value_type;
	};

	template <class T>
	concept _Has_member_element_type = requires
	{
		typename T::element_type;
	};

	template <class T>
	concept _Has_member_difference_type = requires
	{
		typename T::difference_type;
	};

	template <class T>
	concept _Has_member_pointer = requires
	{
		typename T::pointer;
	};

	template <class T>
	concept _Has_member_reference = requires
	{
		typename T::reference;
	};

	template <class>
	struct incrementable_traits {};

	template <class T> requires is_object_v<T>
	struct incrementable_traits<T*>
	{
		using difference_type = ptrdiff_t;
	};

	template <class T>
	struct incrementable_traits<const T> : incrementable_traits<T> {};

	template <_Has_member_difference_type T>
	struct incrementable_traits<T>
	{
		using difference_type = typename T::difference_type;
	};

	//template <class T>
	//concept _Destructible_object = is_object_v<T> && destructible<T>;
}
export namespace std
{
	template <class T>
	constexpr T* addressof(T& value) noexcept;

	template <class T>
	struct pointer_traits {
		using pointer = T;
		using element_type = typename _Get_element_type<T>::type;
		using difference_type = typename _Get_ptr_difference_type<T>::type;

		template <class U>
		using rebind = typename _Get_rebind_alias<T, U>::type;

		using _Reftype = conditional_t<is_void_v<element_type>, char, element_type>&;

		[[nodiscard]] static pointer pointer_to(_Reftype value) noexcept(noexcept(T::pointer_to(value)))
		{
			return T::pointer_to(value);
		}
	};

	template <class T>
	struct pointer_traits<T*> {
		using pointer = T*;
		using element_type = T;
		using difference_type = ptrdiff_t;

		template <class U>
		using rebind = U*;

		using _Reftype = conditional_t<is_void_v<T>, char, T>&;

		[[nodiscard]] static constexpr pointer pointer_to(_Reftype value) noexcept
		{
			return addressof(value);
		}
	};

	struct piecewise_construct_t { explicit piecewise_construct_t() = default; };
	inline constexpr piecewise_construct_t piecewise_construct{};


	template <class T>
	concept _Has_to_address = requires(const T & value) {
		typename pointer_traits<T>;
		pointer_traits<T>::to_address(value);
	};

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag : public random_access_iterator_tag {};

	template <class T>
	[[nodiscard]] constexpr T* to_address(T* const value) noexcept {
		static_assert(!is_function_v<T>, "N4950 [pointer.conversion]/1: Mandates: T is not a function type.");
		return value;
	}

	template <class T>
	[[nodiscard]] constexpr auto to_address(const T& value) noexcept {
		if constexpr (_Has_to_address<T>)
			return pointer_traits<T>::to_address(value);
		else
			return to_address(value.operator->());
	}

	struct identity {
		template <class T>
		[[nodiscard]] constexpr T&& operator()(T&& _Left) const noexcept {
			return forward<T>(_Left);
		}

		using is_transparent = int;
	};

	struct in_place_t
	{
		explicit in_place_t() = default;
	};
	inline constexpr in_place_t in_place{};

	template <class>
	struct in_place_type_t
	{
		explicit in_place_type_t() = default;
	};
	template <class T>
	inline constexpr in_place_type_t<T> in_place_type{};

	template <size_t>
	struct in_place_index_t
	{
		explicit in_place_index_t() = default;
	};
	template <size_t T>
	inline constexpr in_place_index_t<T> in_place_index{};

	[[noreturn]] inline void unreachable() noexcept
	{
		__assume(false);
	}

	template <class T>
	[[nodiscard]] constexpr underlying_type_t<T> to_underlying(T value) noexcept
	{
		return static_cast<underlying_type_t<T>>(value);
	}

	template <class _Iter>
	[[nodiscard]] constexpr void* _Voidify_iter(_Iter _It) noexcept {
		if constexpr (is_pointer_v<_Iter>)
			return const_cast<void*>(static_cast<const volatile void*>(_It));
		else
			return const_cast<void*>(static_cast<const volatile void*>(std::addressof(*_It)));
	}

	template <class _Ty, class... _Types, class = void_t<decltype(::new(std::declval<void*>()) _Ty(std::declval<_Types>()...))>>
	constexpr _Ty* construct_at(_Ty* const _Location, _Types&&... _Args) noexcept(noexcept(::new(_Voidify_iter(_Location)) _Ty(std::forward<_Types>(_Args)...)))
	{
		[[msvc::constexpr]] return ::new (_Voidify_iter(_Location)) _Ty(std::forward<_Types>(_Args)...);
	}
	template <class _Ty, class... _Types>
	constexpr void _Construct_in_place(_Ty& _Obj, _Types&&... _Args) noexcept(
		is_nothrow_constructible_v<_Ty, _Types...>) {
		if (std::is_constant_evaluated()) {
			std::construct_at(std::addressof(_Obj), std::forward<_Types>(_Args)...);
		}
	}

	template <class _Ty>
	concept _Can_difference = requires(const _Ty & __a, const _Ty & __b) {
		{ __a - __b } -> integral;
	};

	template <class _Ty>
		requires (!_Has_member_difference_type<_Ty>&& _Can_difference<_Ty>)
	struct incrementable_traits<_Ty> {
		using difference_type = make_signed_t<decltype(declval<_Ty>() - declval<_Ty>())>;
	};

	template <class _Ty>
	concept _Is_from_primary = _Same_impl<typename _Ty::_From_primary, _Ty>;

	template <class>
	struct iterator_traits;

	template <class _Ty>
	using iter_difference_t = typename conditional_t<_Is_from_primary<iterator_traits<remove_cvref_t<_Ty>>>,
		incrementable_traits<remove_cvref_t<_Ty>>, iterator_traits<remove_cvref_t<_Ty>>>::difference_type;

	template <class>
	struct _Cond_value_type {};

	template <class _Ty>
		requires is_object_v<_Ty>
	struct _Cond_value_type<_Ty> {
		using value_type = remove_cv_t<_Ty>;
	};

	template <class>
	struct indirectly_readable_traits {};

	template <class _Ty>
	struct indirectly_readable_traits<_Ty*> : _Cond_value_type<_Ty> {};

	template <class _Ty>
		requires is_array_v<_Ty>
	struct indirectly_readable_traits<_Ty> {
		using value_type = remove_cv_t<remove_extent_t<_Ty>>;
	};

	template <class _Ty>
	struct indirectly_readable_traits<const _Ty> : indirectly_readable_traits<_Ty> {};

	template <_Has_member_value_type _Ty>
	struct indirectly_readable_traits<_Ty> : _Cond_value_type<typename _Ty::value_type> {};

	template <_Has_member_element_type _Ty>
	struct indirectly_readable_traits<_Ty> : _Cond_value_type<typename _Ty::element_type> {};

	template <_Has_member_value_type _Ty>
		requires _Has_member_element_type<_Ty>
	struct indirectly_readable_traits<_Ty> {};

	template <_Has_member_value_type _Ty>
		requires _Has_member_element_type<_Ty>
	&& same_as<remove_cv_t<typename _Ty::value_type>, remove_cv_t<typename _Ty::element_type>>
		struct indirectly_readable_traits<_Ty> : _Cond_value_type<typename _Ty::value_type> {};

	template <class _Ty>
	using iter_value_t = typename conditional_t<_Is_from_primary<iterator_traits<remove_cvref_t<_Ty>>>,
		indirectly_readable_traits<remove_cvref_t<_Ty>>, iterator_traits<remove_cvref_t<_Ty>>>::value_type;

	template <_Dereferenceable T>
	using iter_reference_t = decltype(*declval<T&>());

	template <class>
	struct _Iterator_traits_base {};

	template <class _It>
	concept _Has_iter_types = _Has_member_difference_type<_It> && _Has_member_value_type<_It> && _Has_member_reference<_It> && _Has_member_iterator_category<_It>;

	template <bool _Has_member_typedef>
	struct _Old_iter_traits_pointer
	{
		template <class _It>
		using _Apply = typename _It::pointer;
	};

	template <>
	struct _Old_iter_traits_pointer<false>
	{
		template <class>
		using _Apply = void;
	};

	template <_Has_iter_types _It>
	struct _Iterator_traits_base<_It> {
		using iterator_category = typename _It::iterator_category;
		using value_type = typename _It::value_type;
		using difference_type = typename _It::difference_type;
		using pointer = typename _Old_iter_traits_pointer<_Has_member_pointer<_It>>::template _Apply<_It>;
		using reference = typename _It::reference;
	};

	template <bool _Has_member_typedef>
	struct _Iter_traits_difference {
		template <class _It>
		using _Apply = typename incrementable_traits<_It>::difference_type;
	};

	template <>
	struct _Iter_traits_difference<false> {
		template <class>
		using _Apply = void;
	};

	// clang-format off
	template <class _It>
	concept _Cpp17_iterator =
		requires(_It __i) {
			{ *__i } -> _Can_reference;
			{ ++__i } -> same_as<_It&>;
			{ *__i++ } -> _Can_reference;
	}
	&& copyable<_It>;

	template <class _It>
	concept _Cpp17_input_iterator = _Cpp17_iterator<_It>
		&& equality_comparable<_It>
		&& _Has_member_difference_type<incrementable_traits<_It>>
		&& _Has_member_value_type<indirectly_readable_traits<_It>>
		&& requires(_It __i) {
		typename common_reference_t<iter_reference_t<_It>&&, typename indirectly_readable_traits<_It>::value_type&>;
		typename common_reference_t<decltype(*__i++)&&, typename indirectly_readable_traits<_It>::value_type&>;
			requires signed_integral<typename incrementable_traits<_It>::difference_type>;
	};

	template <class _It>
		requires (!_Has_iter_types<_It>&& _Cpp17_iterator<_It> && !_Cpp17_input_iterator<_It>)
	struct _Iterator_traits_base<_It> {
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type =
			typename _Iter_traits_difference<_Has_member_difference_type<incrementable_traits<_It>>>::template _Apply<_It>;
		using pointer = void;
		using reference = void;
	};
	// clang-format on

	enum class _Itraits_pointer_strategy { _Use_void, _Use_member, _Use_decltype };

	template <_Itraits_pointer_strategy>
	struct _Iter_traits_pointer;

	template <>
	struct _Iter_traits_pointer<_Itraits_pointer_strategy::_Use_void> {
		template <class>
		using _Apply = void;
	};

	template <>
	struct _Iter_traits_pointer<_Itraits_pointer_strategy::_Use_member> {
		template <class _It>
		using _Apply = typename _It::pointer;
	};

	template <>
	struct _Iter_traits_pointer<_Itraits_pointer_strategy::_Use_decltype> {
		template <class _It>
		using _Apply = decltype(declval<_It&>().operator->());
	};

	template <class _Ty>
	concept _Has_member_arrow = requires(_Ty && __t) { static_cast<_Ty&&>(__t).operator->(); };

	template <bool _Has_member_typedef>
	struct _Iter_traits_reference {
		template <class _It>
		using _Apply = typename _It::reference;
	};

	template <>
	struct _Iter_traits_reference<false> {
		template <class _It>
		using _Apply = iter_reference_t<_It>;
	};

	template <bool _Is_random>
	struct _Iter_traits_category4 {
		using type = random_access_iterator_tag;
	};

	template <>
	struct _Iter_traits_category4<false> {
		using type = bidirectional_iterator_tag;
	};

	template <class _It>
	concept _Cpp17_random_delta = totally_ordered<_It>
		&& requires(_It __i, typename incrementable_traits<_It>::difference_type __n) {
			{ __i += __n } -> same_as<_It&>;
			{ __i -= __n } -> same_as<_It&>;
			{ __i + __n } -> same_as<_It>;
			{ __n + __i } -> same_as<_It>;
			{ __i - __n } -> same_as<_It>;
			{ __i - __i } -> same_as<decltype(__n)>;
			{  __i[__n]  } -> convertible_to<iter_reference_t<_It>>;
	};

	template <bool _Is_bidi>
	struct _Iter_traits_category3 {
		template <class _It>
		using _Apply = typename _Iter_traits_category4<_Cpp17_random_delta<_It>>::type;
	};

	template <>
	struct _Iter_traits_category3<false> {
		template <class>
		using _Apply = forward_iterator_tag;
	};

	template <class _It>
	concept _Cpp17_bidi_delta = requires(_It __i) {
		{ --__i } -> same_as<_It&>;
		{ __i-- } -> convertible_to<const _It&>;
			requires same_as<decltype(*__i--), iter_reference_t<_It>>;
	};

	template <bool _Is_forward>
	struct _Iter_traits_category2 {
		template <class _It>
		using _Apply = typename _Iter_traits_category3<_Cpp17_bidi_delta<_It>>::template _Apply<_It>;
	};

	template <>
	struct _Iter_traits_category2<false> {
		template <class>
		using _Apply = input_iterator_tag;
	};

	template <class _It>
	concept _Cpp17_forward_delta = constructible_from<_It> && is_reference_v<iter_reference_t<_It>>
		&& same_as<remove_cvref_t<iter_reference_t<_It>>, typename indirectly_readable_traits<_It>::value_type>
		&& requires(_It __i) {
			{ __i++ } -> convertible_to<const _It&>;
				requires same_as<decltype(*__i++), iter_reference_t<_It>>;
	};

	template <bool _Has_member_typedef>
	struct _Iter_traits_category {
		template <class _It>
		using _Apply = typename _It::iterator_category;
	};

	template <>
	struct _Iter_traits_category<false> {
		template <class _It>
		using _Apply = typename _Iter_traits_category2<_Cpp17_forward_delta<_It>>::template _Apply<_It>;
	};

	template <class _It>
		requires (!_Has_iter_types<_It>&& _Cpp17_input_iterator<_It>)
	struct _Iterator_traits_base<_It> {
		using iterator_category = typename _Iter_traits_category<_Has_member_iterator_category<_It>>::template _Apply<_It>;
		using value_type = typename indirectly_readable_traits<_It>::value_type;
		using difference_type = typename incrementable_traits<_It>::difference_type;
		using pointer = typename _Iter_traits_pointer<(
			_Has_member_pointer<_It> ? _Itraits_pointer_strategy::_Use_member
			: _Has_member_arrow<_It&> ? _Itraits_pointer_strategy::_Use_decltype
			: _Itraits_pointer_strategy::_Use_void)>::template _Apply<_It>;
		using reference = typename _Iter_traits_reference<_Has_member_reference<_It>>::template _Apply<_It>;
	};

	template <class _Ty>
	struct iterator_traits : _Iterator_traits_base<_Ty> {
		using _From_primary = iterator_traits;
	};

	template <class _Ty>
		requires is_object_v<_Ty>
	struct iterator_traits<_Ty*> {
		using iterator_concept = contiguous_iterator_tag;
		using iterator_category = random_access_iterator_tag;
		using value_type = remove_cv_t<_Ty>;
		using difference_type = ptrdiff_t;
		using pointer = _Ty*;
		using reference = _Ty&;
	};

	template <class _Ty>
	inline constexpr bool _Integer_class = requires {
		typename _Ty::_Signed_type;
		typename _Ty::_Unsigned_type;
	};

	template <class _Ty>
	concept _Integer_like = _Is_nonbool_integral<remove_cv_t<_Ty>> || _Integer_class<_Ty>;

	template <class _Ty>
	concept _Signed_integer_like = _Integer_like<_Ty> && static_cast<_Ty>(-1) < static_cast<_Ty>(0);

	template <class _Ty>
	concept weakly_incrementable = movable<_Ty>
		&& requires(_Ty __i) {
		typename iter_difference_t<_Ty>;
			requires _Signed_integer_like<iter_difference_t<_Ty>>;
		{ ++__i } -> same_as<_Ty&>;
		__i++;
	};

	template <class _It>
	concept input_or_output_iterator = requires(_It __i) { { *__i } -> _Can_reference; }
	&& weakly_incrementable<_It>;

	template <class _Se, class _It>
	concept sentinel_for = semiregular<_Se>
		&& input_or_output_iterator<_It>
		&& _Weakly_equality_comparable_with<_Se, _It>;
	// clang-format on

	template <class _Se, class _It>
	inline constexpr bool disable_sized_sentinel_for = false;

	// clang-format off
	template <class _Se, class _It>
	concept sized_sentinel_for = sentinel_for<_Se, _It>
		&& !disable_sized_sentinel_for<remove_cv_t<_Se>, remove_cv_t<_It>>
		&& requires(const _It& __i, const _Se& __s) {
			{ __s - __i } -> same_as<iter_difference_t<_It>>;
			{ __i - __s } -> same_as<iter_difference_t<_It>>;
	};
	// clang-format on

	struct default_sentinel_t {};
	inline constexpr default_sentinel_t default_sentinel{};

	template <class T> using _Iter_ref_t = iter_reference_t<T>;
	template <class T> using _Iter_value_t = iter_value_t<T>;
	template <class T> using _Iter_diff_t = iter_difference_t<T>;
	template <class... T> using _Common_diff_t = common_type_t<_Iter_diff_t<T>...>;
	template <class T> using _Iter_cat_t = typename iterator_traits<T>::iterator_category;

	template <class _Ty, class = void> inline constexpr bool _Is_iterator_v = false;
	template <class _Ty> inline constexpr bool _Is_iterator_v<_Ty, void_t<_Iter_cat_t<_Ty>>> = true;
	template <class _Ty> struct _Is_iterator : bool_constant<_Is_iterator_v<_Ty>> {};

	template <class... _Types>
	class tuple;

	template <class _Ty1, class _Ty2>
	struct pair;

	template <class _Ty, size_t _Size>
	class array;

	template <class _Tuple>
	struct tuple_size;

	template <class _Ty>
	inline constexpr size_t tuple_size_v = tuple_size<_Ty>::value;

	template <size_t _Index, class _Tuple>
	struct tuple_element;

	template <size_t _Index, class _Tuple>
	using tuple_element_t = typename tuple_element<_Index, _Tuple>::type;

	/* TRANSITION, VSO-1538698 */ template <size_t _Index, class... _Types>
	[[nodiscard]] constexpr auto&& _Tuple_get(tuple<_Types...>&& _Tuple) noexcept;

	template <size_t _Index, class... _Types>
	[[nodiscard]] constexpr tuple_element_t<_Index, tuple<_Types...>>& get(tuple<_Types...>& _Tuple) noexcept;

	template <size_t _Index, class... _Types>
	[[nodiscard]] constexpr const tuple_element_t<_Index, tuple<_Types...>>& get(const tuple<_Types...>& _Tuple) noexcept;

	template <size_t _Index, class... _Types>
	[[nodiscard]] constexpr tuple_element_t<_Index, tuple<_Types...>>&& get(tuple<_Types...>&& _Tuple) noexcept;

	template <size_t _Index, class... _Types>
	[[nodiscard]] constexpr const tuple_element_t<_Index, tuple<_Types...>>&& get(const tuple<_Types...>&& _Tuple) noexcept;

	template <size_t _Idx, class _Ty, size_t _Size>
	[[nodiscard]] constexpr _Ty& get(array<_Ty, _Size>& _Arr) noexcept;

	template <size_t _Idx, class _Ty, size_t _Size>
	[[nodiscard]] constexpr const _Ty& get(const array<_Ty, _Size>& _Arr) noexcept;

	template <size_t _Idx, class _Ty, size_t _Size>
	[[nodiscard]] constexpr _Ty&& get(array<_Ty, _Size>&& _Arr) noexcept;

	template <size_t _Idx, class _Ty, size_t _Size>
	[[nodiscard]] constexpr const _Ty&& get(const array<_Ty, _Size>&& _Arr) noexcept;

	template <class _Ty1, class _Ty2>
	concept _Different_from = (!same_as<remove_cvref_t<_Ty1>, remove_cvref_t<_Ty2>>);

	template <class>
	inline constexpr bool _Is_std_array_v = false;

	template <class _Ty, size_t _Size>
	inline constexpr bool _Is_std_array_v<array<_Ty, _Size>> = true;

	template <class>
	inline constexpr bool _Is_subrange_v = false;

	template <class _Ty>
	inline constexpr bool _Tuple_like_impl =
		_Is_specialization_v<_Ty, tuple> || _Is_specialization_v<_Ty, pair> || _Is_std_array_v<_Ty> || _Is_subrange_v<_Ty>;

	template <class _Ty>
	concept _Tuple_like = _Tuple_like_impl<remove_cvref_t<_Ty>>;

	template <class _Ty>
	concept _Pair_like = _Tuple_like<_Ty> && tuple_size_v<remove_cvref_t<_Ty>> == 2;


	namespace ranges {
		enum class subrange_kind : bool { unsized, sized };

		template <input_or_output_iterator _It, sentinel_for<_It> _Se = _It,
			subrange_kind _Ki = sized_sentinel_for<_Se, _It> ? subrange_kind::sized : subrange_kind::unsized>
			requires (_Ki == subrange_kind::sized || !sized_sentinel_for<_Se, _It>)
		class subrange;

		template <size_t _Idx, class _It, class _Se, subrange_kind _Ki>
			requires ((_Idx == 0 && copyable<_It>) || _Idx == 1)
		[[nodiscard]] constexpr auto get(const subrange<_It, _Se, _Ki>& _Val);

		template <size_t _Idx, class _It, class _Se, subrange_kind _Ki>
			requires (_Idx < 2)
		[[nodiscard]] constexpr auto get(subrange<_It, _Se, _Ki>&& _Val);
	} // namespace ranges

	using ranges::get;

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	inline constexpr bool _Is_subrange_v<ranges::subrange<_It, _Se, _Ki>> = true;

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	struct tuple_size<ranges::subrange<_It, _Se, _Ki>> : integral_constant<size_t, 2> {};

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	struct tuple_element<0, ranges::subrange<_It, _Se, _Ki>> {
		using type = _It;
	};

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	struct tuple_element<1, ranges::subrange<_It, _Se, _Ki>> {
		using type = _Se;
	};

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	struct tuple_element<0, const ranges::subrange<_It, _Se, _Ki>> {
		using type = _It;
	};

	template <class _It, class _Se, ranges::subrange_kind _Ki>
	struct tuple_element<1, const ranges::subrange<_It, _Se, _Ki>> {
		using type = _Se;
	};

	namespace ranges 
	{
		namespace _Iter_move 
		{
			void iter_move();

			template <class _Ty>
			concept _Has_ADL = _Has_class_or_enum_type<_Ty> && requires(_Ty && __t) {
				iter_move(static_cast<_Ty&&>(__t));
			};

			template <class _Ty>
			concept _Can_deref = requires(_Ty && __t) {
				*static_cast<_Ty&&>(__t);
			};
			
			class _Cpo {
			private:
				enum class _St { _None, _Custom, _Fallback };

				template <class _Ty>
				[[nodiscard]] static consteval _Choice_t<_St> _Choose() noexcept {
					if constexpr (_Has_ADL<_Ty>) {
						return { _St::_Custom, noexcept(iter_move(declval<_Ty>())) }; // intentional ADL
					}
					else if constexpr (_Can_deref<_Ty>) {
						return { _St::_Fallback, noexcept(*declval<_Ty>()) };
					}
					else {
						return { _St::_None };
					}
				}

				template <class _Ty>
				static constexpr _Choice_t<_St> _Choice = _Choose<_Ty>();

			public:
				template <class _Ty>
					requires (_Choice<_Ty>._Strategy != _St::_None)
				[[nodiscard]] constexpr decltype(auto) operator()(_Ty&& _Val) const noexcept(_Choice<_Ty>._No_throw) {
					constexpr _St _Strat = _Choice<_Ty>._Strategy;

					if constexpr (_Strat == _St::_Custom)
					{
						return iter_move(static_cast<_Ty&&>(_Val));
					}
					else if constexpr (_Strat == _St::_Fallback)
					{
						using _Ref = decltype(*static_cast<_Ty&&>(_Val));
						if constexpr (is_lvalue_reference_v<_Ref>)
						{
							return move(*static_cast<_Ty&&>(_Val));
						}
						else
						{
							return *static_cast<_Ty&&>(_Val);
						}
					}
					else
					{
						static_assert(_Always_false<_Ty>, "should be unreachable");
					}
				}
			};
		}

		namespace _Cpos
		{
			inline constexpr _Iter_move::_Cpo iter_move;
		}
	}

	template <class _Ty>
		requires _Dereferenceable<_Ty>&& requires(_Ty& __t) {
			{ ranges::iter_move(__t) } -> _Can_reference;
	}
	using iter_rvalue_reference_t = decltype( ranges::iter_move(declval<_Ty&>()));

	template <class _It>
	concept _Indirectly_readable_impl =
		requires(const _It __i) {
		typename iter_value_t<_It>;
		typename iter_reference_t<_It>;
		typename iter_rvalue_reference_t<_It>;
		{ *__i } -> same_as<iter_reference_t<_It>>;
		{ ranges::iter_move(__i) } -> same_as<iter_rvalue_reference_t<_It>>;
	}&& common_reference_with<iter_reference_t<_It>&&, iter_value_t<_It>&>
		&& common_reference_with<iter_reference_t<_It>&&, iter_rvalue_reference_t<_It>&&>
		&& common_reference_with<iter_rvalue_reference_t<_It>&&, const iter_value_t<_It>&>;

	template <class _It>
		concept indirectly_readable = _Indirectly_readable_impl<remove_cvref_t<_It>>;

	template <class _Ty>
	struct _Indirect_value_impl {
		using type = iter_value_t<_Ty>&;
	};

	template <indirectly_readable _It>
	using _Indirect_value_t = typename _Indirect_value_impl<_It>::type;

	template <indirectly_readable _Ty>
		using iter_common_reference_t = common_reference_t<iter_reference_t<_Ty>, _Indirect_value_t<_Ty>>;

	template <class _It, class _Ty>
		concept indirectly_writable = requires(_It && __i, _Ty && __t) {
		*__i = static_cast<_Ty&&>(__t);
		*static_cast<_It&&>(__i) = static_cast<_Ty&&>(__t);
		const_cast<const iter_reference_t<_It>&&>(*__i) = static_cast<_Ty&&>(__t);
		const_cast<const iter_reference_t<_It>&&>(*static_cast<_It&&>(__i)) = static_cast<_Ty&&>(__t);
	};

	template <bool _Is_integer_class>
	struct _Make_unsigned_like_impl {
		template <class _Ty>
		using _Apply = typename _Ty::_Unsigned_type;
	};
	template <>
	struct _Make_unsigned_like_impl<false> {
		template <class _Ty>
		using _Apply = make_unsigned_t<_Ty>;
	};

	template <class _Ty>
	using _Make_unsigned_like_t = typename _Make_unsigned_like_impl<_Integer_class<_Ty>>::template _Apply<_Ty>;

	template <_Integer_like _Ty>
	[[nodiscard]] constexpr auto _To_unsigned_like(const _Ty _Value) noexcept {
		return static_cast<_Make_unsigned_like_t<_Ty>>(_Value);
	}

	template <bool _Is_integer_class>
	struct _Make_signed_like_impl {
		template <class _Ty>
		using _Apply = typename _Ty::_Signed_type;
	};
	template <>
	struct _Make_signed_like_impl<false> {
		template <class _Ty>
		using _Apply = make_signed_t<_Ty>;
	};

	template <class _Ty>
	using _Make_signed_like_t = typename _Make_signed_like_impl<_Integer_class<_Ty>>::template _Apply<_Ty>;

	template <class _Ty>
	concept incrementable = regular<_Ty> && weakly_incrementable<_Ty> && requires(_Ty __t) {
		{ __t++ } -> same_as<_Ty>;
	};

	template <bool _Iterator_category_present>
	struct _Iter_concept_impl2 {
		template <class _It, class _Traits>
		using _Apply = typename _Traits::iterator_category;
	};
	template <>
	struct _Iter_concept_impl2<false> {
		template <class _It, class _Traits>
			requires _Is_from_primary<iterator_traits<_It>>
		using _Apply = random_access_iterator_tag;
	};

	template <bool _Iterator_concept_present>
	struct _Iter_concept_impl1 {
		template <class _It, class _Traits>
		using _Apply = typename _Traits::iterator_concept;
	};
	template <>
	struct _Iter_concept_impl1<false> {
		template <class _It, class _Traits>
		using _Apply = typename _Iter_concept_impl2<_Has_member_iterator_category<_Traits>>::template _Apply<_It, _Traits>;
	};

	template <class _It, class _Traits = conditional_t<_Is_from_primary<iterator_traits<_It>>, _It, iterator_traits<_It>>>
	using _Iter_concept =
		typename _Iter_concept_impl1<_Has_member_iterator_concept<_Traits>>::template _Apply<_It, _Traits>;



	// clang-format off
	template <class _It>
	concept input_iterator = input_or_output_iterator<_It> && indirectly_readable<_It>
		&& requires { typename _Iter_concept<_It>; }
	&& derived_from<_Iter_concept<_It>, input_iterator_tag>;

	template <class _It, class _Ty>
	concept output_iterator = input_or_output_iterator<_It>// && indirectly_writable<_It, _Ty>
		&& requires(_It __i, _Ty && __t) {
		*__i++ = static_cast<_Ty&&>(__t);
	};

	template <class _It>
	concept forward_iterator = input_iterator<_It> && derived_from<_Iter_concept<_It>, forward_iterator_tag>
		&& incrementable<_It>&& sentinel_for<_It, _It>;

	template <class _It>
	concept bidirectional_iterator = forward_iterator<_It> && derived_from<_Iter_concept<_It>, bidirectional_iterator_tag>
		&& requires(_It __i) {
			{ --__i } -> same_as<_It&>;
			{ __i-- } -> same_as<_It>;
	};

	template <class _It>
	concept random_access_iterator = bidirectional_iterator<_It>
		&& derived_from<_Iter_concept<_It>, random_access_iterator_tag>&& totally_ordered<_It>
		&& sized_sentinel_for<_It, _It>&& requires(_It __i, const _It __j, const iter_difference_t<_It> __n) {
			{ __i += __n } -> same_as<_It&>;
			{ __j + __n } -> same_as<_It>;
			{ __n + __j } -> same_as<_It>;
			{ __i -= __n } -> same_as<_It&>;
			{ __j - __n } -> same_as<_It>;
			{ __j[__n] } -> same_as<iter_reference_t<_It>>;
	};

	template <class _It>
	concept contiguous_iterator = random_access_iterator<_It>
		&& derived_from<_Iter_concept<_It>, contiguous_iterator_tag>
		&& is_lvalue_reference_v<iter_reference_t<_It>>
		&& same_as<iter_value_t<_It>, remove_cvref_t<iter_reference_t<_It>>>
		&& requires(const _It& __i) {
			{ to_address(__i) } -> same_as<add_pointer_t<iter_reference_t<_It>>>;
	};

	template <class T> inline constexpr bool _Is_cpp17_input_iter_v = is_convertible_v<_Iter_cat_t<T>, input_iterator_tag>;
	template <class T> inline constexpr bool _Is_ranges_input_iter_v = (input_iterator<T> && sentinel_for<T, T>) || _Is_cpp17_input_iter_v<T>;
	template <class T> inline constexpr bool _Is_cpp17_fwd_iter_v = is_convertible_v<_Iter_cat_t<T>, forward_iterator_tag>;
	template <class T> inline constexpr bool _Is_ranges_fwd_iter_v = forward_iterator<T> || _Is_cpp17_fwd_iter_v<T>;
	template <class T> inline constexpr bool _Is_cpp17_bidi_iter_v = is_convertible_v<_Iter_cat_t<T>, bidirectional_iterator_tag>;
	template <class T> inline constexpr bool _Is_ranges_bidi_iter_v = bidirectional_iterator<T> || _Is_cpp17_bidi_iter_v<T>;
	template <class T> inline constexpr bool _Is_cpp17_random_iter_v = is_convertible_v<_Iter_cat_t<T>, random_access_iterator_tag>;
	template <class T> inline constexpr bool _Is_ranges_random_iter_v = random_access_iterator<T> || _Is_cpp17_random_iter_v<T>;

	template <class T> struct is_execution_policy : false_type {};
	template <class T> inline constexpr bool is_execution_policy_v = is_execution_policy<T>::value;

	template <class T>
	using _Enable_if_execution_policy_t = typename remove_reference_t<T>::_Standard_execution_policy;
}