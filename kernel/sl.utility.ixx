export module sl.utility;
import sl.typetraits;
import sl.bit;
import sl.type;
import sl.concepts;
import sl.compare;
import sl.typetraits;
import sl.iterator_core;
import serial;
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
		[[nodiscard]] constexpr T&& operator()(T&& left) const noexcept {
			return forward<T>(left);
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

	export template <class T, class... _Types, class = void_t<decltype(::new(std::declval<void*>()) T(std::declval<_Types>()...))>>
	constexpr T* construct_at(T* const _Location, _Types&&... _Args) noexcept(noexcept(::new(_Voidify_iter(_Location)) T(std::forward<_Types>(_Args)...)))
	{
		[[msvc::constexpr]] return ::new (_Voidify_iter(_Location)) T(std::forward<_Types>(_Args)...);
	}
	export template <class T, class... _Types>
	constexpr void _Construct_in_place(T& _Obj, _Types&&... _Args) noexcept(is_nothrow_constructible_v<T, _Types...>) {
		if (std::is_constant_evaluated()) {
			std::construct_at(std::addressof(_Obj), std::forward<_Types>(_Args)...);
		}
		else
		{
			::new (static_cast<void*>(std::addressof(_Obj))) T(std::forward<_Types>(_Args)...);
		}
	}

	template <class T>
	concept _Can_difference = requires(const T & __a, const T & __b) {
		{ __a - __b } -> integral;
	};

	template <class T>
		requires (!_Has_member_difference_type<T>&& _Can_difference<T>)
	struct incrementable_traits<T> {
		using difference_type = make_signed_t<decltype(declval<T>() - declval<T>())>;
	};

	template <class T>
	concept _Is_from_primary = _Same_impl<typename T::_From_primary, T>;

	template <class>
	struct iterator_traits;

	template <class T>
	using iter_difference_t = typename conditional_t<_Is_from_primary<iterator_traits<remove_cvref_t<T>>>,
		incrementable_traits<remove_cvref_t<T>>, iterator_traits<remove_cvref_t<T>>>::difference_type;

	template <class>
	struct _Cond_value_type {};

	template <class T>
		requires is_object_v<T>
	struct _Cond_value_type<T> {
		using value_type = remove_cv_t<T>;
	};

	template <class>
	struct indirectly_readable_traits {};

	template <class T>
	struct indirectly_readable_traits<T*> : _Cond_value_type<T> {};

	template <class T>
		requires is_array_v<T>
	struct indirectly_readable_traits<T> {
		using value_type = remove_cv_t<remove_extent_t<T>>;
	};

	template <class T>
	struct indirectly_readable_traits<const T> : indirectly_readable_traits<T> {};

	template <_Has_member_value_type T>
	struct indirectly_readable_traits<T> : _Cond_value_type<typename T::value_type> {};

	template <_Has_member_element_type T>
	struct indirectly_readable_traits<T> : _Cond_value_type<typename T::element_type> {};

	template <_Has_member_value_type T>
		requires _Has_member_element_type<T>
	struct indirectly_readable_traits<T> {};

	template <_Has_member_value_type T>
		requires _Has_member_element_type<T>
	&& same_as<remove_cv_t<typename T::value_type>, remove_cv_t<typename T::element_type>>
		struct indirectly_readable_traits<T> : _Cond_value_type<typename T::value_type> {};

	template <class T>
	using iter_value_t = typename conditional_t<_Is_from_primary<iterator_traits<remove_cvref_t<T>>>,
		indirectly_readable_traits<remove_cvref_t<T>>, iterator_traits<remove_cvref_t<T>>>::value_type;

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

	template <class T>
	concept _Has_member_arrow = requires(T && __t) { static_cast<T&&>(__t).operator->(); };

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

	template <class T>
	struct iterator_traits : _Iterator_traits_base<T> {
		using _From_primary = iterator_traits;
	};

	template <class T>
		requires is_object_v<T>
	struct iterator_traits<T*> {
		using iterator_concept = contiguous_iterator_tag;
		using iterator_category = random_access_iterator_tag;
		using value_type = remove_cv_t<T>;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};

	template <class T>
	inline constexpr bool _Integer_class = requires {
		typename T::_Signed_type;
		typename T::_Unsigned_type;
	};

	template <class T>
	concept _Integer_like = _Is_nonbool_integral<remove_cv_t<T>> || _Integer_class<T>;

	template <class T>
	concept _Signed_integer_like = _Integer_like<T> && static_cast<T>(-1) < static_cast<T>(0);

	template <class T>
	concept weakly_incrementable = movable<T>
		&& requires(T __i) {
		typename iter_difference_t<T>;
			requires _Signed_integer_like<iter_difference_t<T>>;
		{ ++__i } -> same_as<T&>;
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

	template <class T> using _Iter_ref_t = iter_reference_t<T>;
	template <class T> using _Iter_value_t = iter_value_t<T>;
	template <class T> using _Iter_diff_t = iter_difference_t<T>;
	template <class... T> using _Common_diff_t = common_type_t<_Iter_diff_t<T>...>;
	template <class T> using _Iter_cat_t = typename iterator_traits<T>::iterator_category;

	template <class T, class = void> inline constexpr bool _Is_iterator_v = false;
	template <class T> inline constexpr bool _Is_iterator_v<T, void_t<_Iter_cat_t<T>>> = true;
	template <class T> struct _Is_iterator : bool_constant<_Is_iterator_v<T>> {};

	template <class... _Types>
	class tuple;

	template <class _Ty1, class _Ty2>
	struct pair;

	template <class T, size_t _Size>
	class array;

	template <class _Tuple>
	struct tuple_size;

	template <class T>
	inline constexpr size_t tuple_size_v = tuple_size<T>::value;

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

	template <size_t _Idx, class T, size_t _Size>
	[[nodiscard]] constexpr T& get(array<T, _Size>& _Arr) noexcept;

	template <size_t _Idx, class T, size_t _Size>
	[[nodiscard]] constexpr const T& get(const array<T, _Size>& _Arr) noexcept;

	template <size_t _Idx, class T, size_t _Size>
	[[nodiscard]] constexpr T&& get(array<T, _Size>&& _Arr) noexcept;

	template <size_t _Idx, class T, size_t _Size>
	[[nodiscard]] constexpr const T&& get(const array<T, _Size>&& _Arr) noexcept;

	template <class _Ty1, class _Ty2>
	concept _Different_from = (!same_as<remove_cvref_t<_Ty1>, remove_cvref_t<_Ty2>>);

	template <class>
	inline constexpr bool _Is_std_array_v = false;

	template <class T, size_t _Size>
	inline constexpr bool _Is_std_array_v<array<T, _Size>> = true;

	template <class>
	inline constexpr bool _Is_subrange_v = false;

	template <class T>
	inline constexpr bool _Tuple_like_impl =
		_Is_specialization_v<T, tuple> || _Is_specialization_v<T, pair> || _Is_std_array_v<T> || _Is_subrange_v<T>;

	template <class T>
	concept _Tuple_like = _Tuple_like_impl<remove_cvref_t<T>>;

	template <class T>
	concept _Pair_like = _Tuple_like<T> && tuple_size_v<remove_cvref_t<T>> == 2;


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

			template <class T>
			concept _Has_ADL = _Has_class_or_enum_type<T> && requires(T && __t) {
				iter_move(static_cast<T&&>(__t));
			};

			template <class T>
			concept _Can_deref = requires(T && __t) {
				*static_cast<T&&>(__t);
			};
			
			class _Cpo {
			private:
				enum class _St { _None, _Custom, _Fallback };

				template <class T>
				[[nodiscard]] static consteval _Choice_t<_St> _Choose() noexcept {
					if constexpr (_Has_ADL<T>) {
						return { _St::_Custom, noexcept(iter_move(declval<T>())) }; // intentional ADL
					}
					else if constexpr (_Can_deref<T>) {
						return { _St::_Fallback, noexcept(*declval<T>()) };
					}
					else {
						return { _St::_None };
					}
				}

				template <class T>
				static constexpr _Choice_t<_St> _Choice = _Choose<T>();

			public:
				template <class T>
					requires (_Choice<T>._Strategy != _St::_None)
				[[nodiscard]] constexpr decltype(auto) operator()(T&& _Val) const noexcept(_Choice<T>._No_throw) {
					constexpr _St _Strat = _Choice<T>._Strategy;

					if constexpr (_Strat == _St::_Custom)
					{
						return iter_move(static_cast<T&&>(_Val));
					}
					else if constexpr (_Strat == _St::_Fallback)
					{
						using _Ref = decltype(*static_cast<T&&>(_Val));
						if constexpr (is_lvalue_reference_v<_Ref>)
						{
							return move(*static_cast<T&&>(_Val));
						}
						else
						{
							return *static_cast<T&&>(_Val);
						}
					}
					else
					{
						static_assert(_Always_false<T>, "should be unreachable");
					}
				}
			};
		}

		namespace _Cpos
		{
			inline constexpr _Iter_move::_Cpo iter_move;
		}
	}

	template <class T>
		requires _Dereferenceable<T>&& requires(T& __t) {
			{ ranges::iter_move(__t) } -> _Can_reference;
	}
	using iter_rvalue_reference_t = decltype( ranges::iter_move(declval<T&>()));

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

	template <class T>
	struct _Indirect_value_impl {
		using type = iter_value_t<T>&;
	};

	template <indirectly_readable _It>
	using _Indirect_value_t = typename _Indirect_value_impl<_It>::type;

	template <indirectly_readable T>
		using iter_common_reference_t = common_reference_t<iter_reference_t<T>, _Indirect_value_t<T>>;

	template <class _It, class T>
		concept indirectly_writable = requires(_It && __i, T && __t) {
		*__i = static_cast<T&&>(__t);
		*static_cast<_It&&>(__i) = static_cast<T&&>(__t);
		const_cast<const iter_reference_t<_It>&&>(*__i) = static_cast<T&&>(__t);
		const_cast<const iter_reference_t<_It>&&>(*static_cast<_It&&>(__i)) = static_cast<T&&>(__t);
	};

	template <bool _Is_integer_class>
	struct _Make_unsigned_like_impl {
		template <class T>
		using _Apply = typename T::_Unsigned_type;
	};
	template <>
	struct _Make_unsigned_like_impl<false> {
		template <class T>
		using _Apply = make_unsigned_t<T>;
	};

	template <class T>
	using _Make_unsigned_like_t = typename _Make_unsigned_like_impl<_Integer_class<T>>::template _Apply<T>;

	template <_Integer_like T>
	[[nodiscard]] constexpr auto _To_unsigned_like(const T _Value) noexcept {
		return static_cast<_Make_unsigned_like_t<T>>(_Value);
	}

	template <bool _Is_integer_class>
	struct _Make_signed_like_impl {
		template <class T>
		using _Apply = typename T::_Signed_type;
	};
	template <>
	struct _Make_signed_like_impl<false> {
		template <class T>
		using _Apply = make_signed_t<T>;
	};

	template <class T>
	using _Make_signed_like_t = typename _Make_signed_like_impl<_Integer_class<T>>::template _Apply<T>;

	template <class T>
	concept incrementable = regular<T> && weakly_incrementable<T> && requires(T __t) {
		{ __t++ } -> same_as<T>;
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

	template <class _It, class T>
	concept output_iterator = input_or_output_iterator<_It>// && indirectly_writable<_It, T>
		&& requires(_It __i, T && __t) {
		*__i++ = static_cast<T&&>(__t);
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

	template <class _Iter>
	constexpr bool _Iterator_is_contiguous = contiguous_iterator<_Iter>;

	template <class _Iter>
	[[nodiscard]] constexpr auto _To_address(const _Iter& _Val) noexcept
	{
		return to_address(_Val);
	}

	template <class _Elem, bool _Is_enum = is_enum_v<_Elem>>
	struct _Unwrap_enum { // if _Elem is an enum, gets its underlying type; otherwise leaves _Elem unchanged
		using type = underlying_type_t<_Elem>;
	};

	template <class _Elem>
	struct _Unwrap_enum<_Elem, false> { // passthrough non-enum type
		using type = _Elem;
	};

	template <class _Elem>
	using _Unwrap_enum_t = typename _Unwrap_enum<_Elem>::type;

	template <class _Source, class _Dest, class _SourceRef, class _DestRef>
	struct _Trivial_cat {
		using _USource = _Unwrap_enum_t<_Source>;
		using _UDest = _Unwrap_enum_t<_Dest>;

		static constexpr bool _Same_size_and_compatible =
			sizeof(_Source) == sizeof(_Dest)
			// If _UDest is bool, _USource also needs to be bool
			// Conversion from non-bool => non-bool | bool => bool | bool => non-bool is fine.
			// Conversion from non-bool => bool is not fine.
			&& is_same_v<bool, _USource> >= is_same_v<bool, _UDest>
			&& (is_same_v<_USource, _UDest> || (is_integral_v<_USource> && is_integral_v<_UDest>)
				|| (is_floating_point_v<_USource> && is_floating_point_v<_UDest>));

		static constexpr bool _Bitcopy_constructible =
			_Same_size_and_compatible && is_trivially_constructible_v<_Dest, _SourceRef>;

		static constexpr bool _Bitcopy_assignable =
			_Same_size_and_compatible && is_trivially_assignable_v<_DestRef, _SourceRef>;
	};

	template <class _Source, class _Dest>
	constexpr bool _Is_pointer_address_convertible = is_void_v<_Source>
		|| is_void_v<_Dest>
		|| is_same_v<remove_cv_t<_Source>, remove_cv_t<_Dest>>
		|| is_pointer_interconvertible_base_of_v<_Dest, _Source>

		template <class _Source, class _Dest, class _SourceRef, class _DestRef>
	struct _Trivial_cat<_Source*, _Dest*, _SourceRef, _DestRef> {
		static constexpr bool _Bitcopy_constructible =
			_Is_pointer_address_convertible<_Source, _Dest>&& is_trivially_constructible_v<_Dest*, _SourceRef>;

		static constexpr bool _Bitcopy_assignable =
			_Is_pointer_address_convertible<_Source, _Dest>&& is_trivially_assignable_v<_DestRef, _SourceRef>;
	};

	struct _False_trivial_cat {
		static constexpr bool _Bitcopy_constructible = false;
		static constexpr bool _Bitcopy_assignable = false;
	};

	template <class _Iter1, class _Iter2>
	constexpr bool _Iterators_are_contiguous = _Iterator_is_contiguous<_Iter1> && _Iterator_is_contiguous<_Iter2>;

	template <class _Iter>
	constexpr bool _Iterator_is_volatile = is_volatile_v<remove_reference_t<_Iter_ref_t<_Iter>>>;

	struct _Default_sentinel {}; // empty struct to serve as the end of a range

	template <semiregular>
	class move_sentinel;

	template <class>
	struct _Move_iterator_category {};

	template <class _Iter>
		requires requires { typename _Iter_cat_t<_Iter>; }
	struct _Move_iterator_category<_Iter> {
		using iterator_category = conditional_t<derived_from<_Iter_cat_t<_Iter>, random_access_iterator_tag>,
			random_access_iterator_tag, _Iter_cat_t<_Iter>>;
	};

	template <class _Iter>
	class move_iterator : public _Move_iterator_category<_Iter> {
	private:
		_Iter _Current{};

	public:
		using iterator_type = _Iter;
		using value_type = _Iter_value_t<_Iter>;
		using difference_type = _Iter_diff_t<_Iter>;
		using pointer = _Iter;

	private:
		static constexpr auto _Get_iter_concept() {
			if constexpr (random_access_iterator<_Iter>) {
				return random_access_iterator_tag{};
			}
			else if constexpr (bidirectional_iterator<_Iter>) {
				return bidirectional_iterator_tag{};
			}
			else if constexpr (forward_iterator<_Iter>) {
				return forward_iterator_tag{};
			}
			else {
				return input_iterator_tag{};
			}
		}

	public:
		using iterator_concept = decltype(_Get_iter_concept());

		using reference = iter_rvalue_reference_t<_Iter>;

		constexpr move_iterator() = default;

		constexpr explicit move_iterator(_Iter right) noexcept(is_nothrow_move_constructible_v<_Iter>) : _Current(std::move(right)) {}

		template <class _Other> requires (!is_same_v<_Other, _Iter>) && convertible_to<const _Other&, _Iter>
		constexpr move_iterator(const move_iterator<_Other>& right) noexcept(is_nothrow_constructible_v<_Iter, const _Other&>) : _Current(right.base()) {}

		template <class _Other> requires (!is_same_v<_Other, _Iter>) && convertible_to<const _Other&, _Iter>&& assignable_from<_Iter&, const _Other&>
		constexpr move_iterator& operator=(const move_iterator<_Other>& right) noexcept(is_nothrow_assignable_v<_Iter&, const _Other&>)
		{
			_Current = right.base();
			return *this;
		}

		[[nodiscard]] constexpr const iterator_type& base() const& noexcept
		{
			return _Current;
		}
		[[nodiscard]] constexpr iterator_type base() && noexcept(is_nothrow_move_constructible_v<_Iter>)
		{
			return std::move(_Current);
		}

		/*[[nodiscard]] constexpr reference operator*() const noexcept(noexcept(ranges::iter_move(_Current))
		{
			return ranges::iter_move(_Current);
		}*/

		constexpr move_iterator& operator++() noexcept(noexcept(++_Current))
		{
			++_Current;
			return *this;
		}

		constexpr auto operator++(int) noexcept(is_nothrow_copy_constructible_v<_Iter> && noexcept(++_Current))
		{
			if constexpr (forward_iterator<_Iter>) {
				move_iterator _Tmp = *this;
				++_Current;
				return _Tmp;
			}
			else {
				++_Current;
			}
		}

		constexpr move_iterator& operator--() noexcept(noexcept(--_Current))
		{
			--_Current;
			return *this;
		}

		constexpr move_iterator operator--(int) noexcept(is_nothrow_copy_constructible_v<_Iter> && noexcept(--_Current))
		{
			move_iterator _Tmp = *this;
			--_Current;
			return _Tmp;
		}

		template <class _Iter2 = _Iter>
		[[nodiscard]] auto operator==(_Default_sentinel _Sentinel) const noexcept -> decltype(std::declval<const _Iter2&>() == _Sentinel) {
			return _Current == _Sentinel;
		}

		template <class _Iter2 = _Iter>
		[[nodiscard]] auto operator!=(_Default_sentinel _Sentinel) const noexcept -> decltype(std::declval<const _Iter2&>() != _Sentinel) {
			return _Current != _Sentinel;
		}

		[[nodiscard]] constexpr move_iterator operator+(const difference_type _Off) const noexcept(noexcept(move_iterator(_Current + _Off)))
		{
			return move_iterator(_Current + _Off);
		}

		constexpr move_iterator& operator+=(const difference_type _Off) noexcept(noexcept(_Current += _Off))
		{
			_Current += _Off;
			return *this;
		}

		[[nodiscard]] constexpr move_iterator operator-(const difference_type _Off) const noexcept(noexcept(move_iterator(_Current - _Off)))
		{
			return move_iterator(_Current - _Off);
		}

		constexpr move_iterator& operator-=(const difference_type _Off) noexcept(noexcept(_Current -= _Off))
		{
			_Current -= _Off;
			return *this;
		}

		/*[[nodiscard]] constexpr reference operator[](const difference_type _Off) const noexcept(noexcept(ranges::iter_move(_Current + _Off)))
		{
			return ranges::iter_move(_Current + _Off);
		}*/

		template <sentinel_for<_Iter> _Sent>
		[[nodiscard]] friend constexpr bool
			operator==(const move_iterator& left, const move_sentinel<_Sent>& right) noexcept(
				noexcept(std::_Fake_copy_init<bool>(left._Current == right._Get_last()))) {
			return left._Current == right._Get_last();
		}

		template <sized_sentinel_for<_Iter> _Sent>
		[[nodiscard]] friend constexpr difference_type operator-(const move_sentinel<_Sent>& left,
			const move_iterator& right) noexcept(noexcept(left._Get_last() - right._Current)) {
			return left._Get_last() - right._Current;
		}

		template <sized_sentinel_for<_Iter> _Sent>
		[[nodiscard]] friend constexpr difference_type operator-(const move_iterator& left,
			const move_sentinel<_Sent>& right) noexcept(noexcept(left._Current - right._Get_last())) {
			return left._Current - right._Get_last();
		}

		/*[[nodiscard]] friend constexpr reference iter_move(const move_iterator& _It) noexcept(
			noexcept(ranges::iter_move(_It._Current))) {
			return ranges::iter_move(_It._Current);
		}

		template <indirectly_swappable<_Iter> _Iter2>
		friend constexpr void iter_swap(const move_iterator& left, const move_iterator<_Iter2>& right) noexcept(
			noexcept(ranges::iter_swap(left._Current, right.base()))) {
			ranges::iter_swap(left._Current, right.base());
		}*/

		/*template <class _Iter2, enable_if_t<_Range_verifiable_v<_Iter, _Iter2>, int> = 0>
		friend constexpr void _Verify_range(const move_iterator& _First, const move_iterator<_Iter2>& _Last) noexcept {
			_Verify_range(_First._Current, _Last._Get_current());
		}

		template <sentinel_for<_Iter> _Sent>
			requires _Range_verifiable_v<_Iter, _Sent>
		friend constexpr void _Verify_range(const move_iterator& _First, const move_sentinel<_Sent>& _Last) noexcept {
			_Verify_range(_First._Current, _Last._Get_last());
		}*/

		using _Prevent_inheriting_unwrap = move_iterator;

		/*template <class _Iter2 = iterator_type, enable_if_t<_Offset_verifiable_v<_Iter2>, int> = 0>
		constexpr void _Verify_offset(const difference_type _Off) const noexcept {
			_Current._Verify_offset(_Off);
		}

		template <class _Iter2 = iterator_type, enable_if_t<_Unwrappable_v<const _Iter2&>, int> = 0>
		[[nodiscard]] constexpr move_iterator<_Unwrapped_t<const _Iter2&>> _Unwrapped() const& noexcept(
			noexcept(static_cast<move_iterator<_Unwrapped_t<const _Iter2&>>>(_Current._Unwrapped()))) {
			return static_cast<move_iterator<_Unwrapped_t<const _Iter2&>>>(_Current._Unwrapped());
		}
		template <class _Iter2 = iterator_type, enable_if_t<_Unwrappable_v<_Iter2>, int> = 0>
		[[nodiscard]] constexpr move_iterator<_Unwrapped_t<_Iter2>> _Unwrapped() && noexcept(
			noexcept(static_cast<move_iterator<_Unwrapped_t<_Iter2>>>(std::move(_Current)._Unwrapped()))) {
			return static_cast<move_iterator<_Unwrapped_t<_Iter2>>>(std::move(_Current)._Unwrapped());
		}*/

		/*static constexpr bool _Unwrap_when_unverified = _Do_unwrap_when_unverified_v<iterator_type>;*/

		/*template <class _Src, enable_if_t<_Wrapped_seekable_v<iterator_type, const _Src&>, int> = 0>
		constexpr void _Seek_to(const move_iterator<_Src>& _It) noexcept(noexcept(_Current._Seek_to(_It._Get_current()))) {
			_Current._Seek_to(_It._Get_current());
		}
		template <class _Src, enable_if_t<_Wrapped_seekable_v<iterator_type, _Src>, int> = 0>
		constexpr void _Seek_to(move_iterator<_Src>&& _It) noexcept(
			noexcept(_Current._Seek_to(std::move(_It)._Get_current()))) {
			_Current._Seek_to(std::move(_It)._Get_current());
		}*/

		[[nodiscard]] constexpr const iterator_type& _Get_current() const& noexcept {
			return _Current;
		}
		[[nodiscard]] constexpr iterator_type&& _Get_current() && noexcept {
			return std::move(_Current);
		}
	};

	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr bool
		operator==(const move_iterator<_Iter1>& left, const move_iterator<_Iter2>& right) noexcept(
			noexcept(std::_Fake_copy_init<bool>(left.base() == right.base())))
		requires requires {
			{ left.base() == right.base() } -> _Implicitly_convertible_to<bool>;
	}
	{
		return left.base() == right.base();
	}


	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr bool
		operator<(const move_iterator<_Iter1>& left, const move_iterator<_Iter2>& right) noexcept(
			noexcept(std::_Fake_copy_init<bool>(left.base() < right.base())))
		requires requires {
			{ left.base() < right.base() } -> _Implicitly_convertible_to<bool>;
	}
	{
		return left.base() < right.base();
	}

	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr bool operator>(const move_iterator<_Iter1>& left,
		const move_iterator<_Iter2>& right) noexcept(noexcept(right < left))
		requires requires { right < left; }
	{
		return right < left;
	}

	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr bool operator<=(const move_iterator<_Iter1>& left,
		const move_iterator<_Iter2>& right) noexcept(noexcept(right < left))
		requires requires { right < left; }
	{
		return !(right < left);
	}

	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr bool operator>=(const move_iterator<_Iter1>& left,
		const move_iterator<_Iter2>& right) noexcept(noexcept(left < right))
		requires requires { left < right; }
	{
		return !(left < right);
	}

	template <class _Iter1, three_way_comparable_with<_Iter1> _Iter2>
	[[nodiscard]] constexpr compare_three_way_result_t<_Iter1, _Iter2> operator<=>(const move_iterator<_Iter1>& left,
		const move_iterator<_Iter2>& right) noexcept(noexcept(left.base() <=> right.base())) {
		return left.base() <=> right.base();
	}


	template <class _Iter1, class _Iter2>
	[[nodiscard]] constexpr auto operator-(const move_iterator<_Iter1>& left,
		const move_iterator<_Iter2>& right) noexcept(noexcept(left.base() - right.base()))
		-> decltype(left.base() - right.base()) {
		return left.base() - right.base();
	}

	template <class _Iter>
	[[nodiscard]] constexpr move_iterator<_Iter>
		operator+(typename move_iterator<_Iter>::difference_type _Off, const move_iterator<_Iter>& right) noexcept(
			noexcept(move_iterator<_Iter>(right.base() + _Off)))
		requires requires {
			{ right.base() + _Off } -> same_as<_Iter>;
	}
	{
		return move_iterator<_Iter>(right.base() + _Off);
	}

	template <class _Iter>
	[[nodiscard]] constexpr move_iterator<_Iter> make_move_iterator(_Iter _It) noexcept(
		is_nothrow_move_constructible_v<_Iter>) {
		return move_iterator<_Iter>(std::move(_It));
	}

	template <class _SourceIt, class _DestIt, bool _Are_contiguous = _Iterators_are_contiguous<_SourceIt, _DestIt> && !_Iterator_is_volatile<_SourceIt> && !_Iterator_is_volatile<_DestIt>>
	struct _Iter_move_cat : _Trivial_cat<_Iter_value_t<_SourceIt>, _Iter_value_t<_DestIt>, remove_reference_t<_Iter_ref_t<_SourceIt>>&&, _Iter_ref_t<_DestIt>> {};

	template <class _SourceIt, class _DestIt>
	struct _Iter_move_cat<_SourceIt, _DestIt, false> : _False_trivial_cat {};

	template <class _SourceIt, class _DestIt>
	struct _Iter_move_cat<move_iterator<_SourceIt>, _DestIt, false> : _Iter_move_cat<_SourceIt, _DestIt> {};

	template <class _SourceIt, class _DestIt,
		bool _Are_contiguous = _Iterators_are_contiguous<_SourceIt, _DestIt> && !_Iterator_is_volatile<_SourceIt> && !_Iterator_is_volatile<_DestIt>>
		struct _Iter_copy_cat : _Trivial_cat<_Iter_value_t<_SourceIt>, _Iter_value_t<_DestIt>, _Iter_ref_t<_SourceIt>, _Iter_ref_t<_DestIt>> {};

	template <class _SourceIt, class _DestIt>
	struct _Iter_copy_cat<_SourceIt, _DestIt, false> : _False_trivial_cat {};

	template <class _SourceIt, class _DestIt>
	struct _Iter_copy_cat<move_iterator<_SourceIt>, _DestIt, false> : _Iter_move_cat<_SourceIt, _DestIt> {};

	template <class _InIt, class _Sent, class _OutIt>
	using _Sent_copy_cat = conditional_t<is_same_v<_Sent, _InIt> || sized_sentinel_for<_Sent, _InIt>, _Iter_copy_cat<_InIt, _OutIt>, _False_trivial_cat>;

	template <class _CtgIt, class _OutCtgIt>
	_OutCtgIt _Copy_memmove(_CtgIt _First, _CtgIt _Last, _OutCtgIt _Dest) {
		auto _FirstPtr = _To_address(_First);
		auto _LastPtr = _To_address(_Last);
		auto _DestPtr = _To_address(_Dest);
		const char* const _First_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(_FirstPtr));
		const char* const _Last_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(_LastPtr));
		char* const _Dest_ch = const_cast<char*>(reinterpret_cast<const volatile char*>(_DestPtr));
		const auto _Count = static_cast<size_t>(_Last_ch - _First_ch);
		::memmove(_Dest_ch, _First_ch, _Count);
		if constexpr (is_pointer_v<_OutCtgIt>) {
			return reinterpret_cast<_OutCtgIt>(_Dest_ch + _Count);
		}
		else {
			return _Dest + (_LastPtr - _FirstPtr);
		}
	}

	template <class _CtgIt, class _OutCtgIt>
	_OutCtgIt _Copy_memmove_n(_CtgIt _First, const size_t _Count, _OutCtgIt _Dest) {
		const auto _Result = _Copy_memmove(_First, _First + _Count, _Dest);
		if constexpr (is_pointer_v<_OutCtgIt>) {
			return _Result;
		}
		else { // _Result is unused so the compiler can optimize it away
			return _Dest + static_cast<_Iter_diff_t<_OutCtgIt>>(_Count);
		}
	}

	template <class _It, bool _RequiresMutable = false>
	constexpr bool _Is_vb_iterator = false;

	template <class _VbIt, class _OutIt>
	constexpr _OutIt _Copy_vbool(_VbIt _First, _VbIt _Last, _OutIt _Dest);

	template <class _VbIt>
	[[nodiscard]] constexpr _Iter_diff_t<_VbIt> _Count_vbool(_VbIt _First, _VbIt _Last, bool _Val) noexcept;

	template <class _VbIt>
	constexpr void _Fill_vbool(_VbIt _First, _VbIt _Last, bool _Val) noexcept;

	template <class _VbIt>
	[[nodiscard]] constexpr _VbIt _Find_vbool(_VbIt _First, _VbIt _Last, bool _Val) noexcept;

	template <class _InIt, class _SizeTy, class _OutIt>
	constexpr _OutIt _Copy_n_unchecked4(_InIt _First, _SizeTy _Count, _OutIt _Dest) {
		if constexpr (_Iter_copy_cat<_InIt, _OutIt>::_Bitcopy_assignable) {
			if (!is_constant_evaluated())
			{
				return _Copy_memmove_n(_First, static_cast<size_t>(_Count), _Dest);
			}
		}
		for (; _Count != 0; ++_Dest, (void) ++_First, --_Count) {
			*_Dest = *_First;
		}

		return _Dest;
	}

	struct _Distance_unknown {
		constexpr _Distance_unknown operator-() const noexcept {
			return {};
		}
	};

	template <class _Checked, class _Iter>
	[[nodiscard]] constexpr auto _Idl_distance(const _Iter& _First, const _Iter& _Last) {
		// tries to get the distance between _First and _Last if they are random-access iterators
		if constexpr (_Is_ranges_random_iter_v<_Iter>) {
			return static_cast<_Iter_diff_t<_Checked>>(_Last - _First);
		}
		else {
			return _Distance_unknown{};
		}
	}

	template <class _InIt, class _Sent, class _OutIt>
	constexpr _OutIt _Copy_unchecked(_InIt _First, _Sent _Last, _OutIt _Dest) {
		// copy [_First, _Last) to [_Dest, ...)
		// note: _Copy_unchecked has callers other than the copy family
		if constexpr (_Is_vb_iterator<_InIt> && _Is_vb_iterator<_OutIt, true>) {
			return _Copy_vbool(_First, _Last, _Dest);
		}
		else {
			if constexpr (_Sent_copy_cat<_InIt, _Sent, _OutIt>::_Bitcopy_assignable) {
				if (!is_constant_evaluated())
				{
					if constexpr (!is_same_v<_InIt, _Sent>) {
						return _Copy_memmove_n(_First, static_cast<size_t>(_Last - _First), _Dest);
					}
					else
					{
						return _Copy_memmove(_First, _Last, _Dest);
					}
				}
			}

			for (; _First != _Last; ++_Dest, (void) ++_First) {
				*_Dest = *_First;
			}

			return _Dest;
		}
	}

	template <class _InIt, class _OutIt>
	constexpr _OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest)
	{
		return _Copy_unchecked(_First, _Last, _Dest);
	}

	template <class _ExPo, class _FwdIt1, class _FwdIt2, _Enable_if_execution_policy_t<_ExPo> = 0>
	_FwdIt2 copy(_ExPo&&, _FwdIt1 _First, _FwdIt1 _Last, _FwdIt2 _Dest) noexcept
	{
		return copy(_First, _Last, _Dest);
	}
}