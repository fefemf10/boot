export module sl.utility;
import sl.typetraits;
import sl.bit;
import sl.type;
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
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag : public random_access_iterator_tag {};

	/*template <class _Ty>
	concept _Is_from_primary = _Same_impl<typename _Ty::_From_primary, _Ty>;

	template <class>
	struct iterator_traits;

	template <class _Ty>
	using iter_difference_t = typename conditional_t<_Is_from_primary<iterator_traits<remove_cvref_t<_Ty>>>,
		incrementable_traits<remove_cvref_t<_Ty>>, iterator_traits<remove_cvref_t<_Ty>>>::difference_type;

	template <class _Ty>
	concept weakly_incrementable = movable<_Ty>
		&& requires(_Ty __i) {
		typename iter_difference_t<_Ty>;
			requires _Signed_integer_like<iter_difference_t<_Ty>>;
		{ ++__i } -> same_as<_Ty&>;
		__i++;
	};

	template <class _Ty>
	concept incrementable = regular<_Ty> && weakly_incrementable<_Ty> && requires(_Ty __t) {
		{ __t++ } -> same_as<_Ty>;
	};

	template <class _It>
	concept input_or_output_iterator = requires(_It __i) { { *__i } -> _Can_reference; }
	&& weakly_incrementable<_It>;

	template <class _It>
	concept input_iterator = input_or_output_iterator<_It> && indirectly_readable<_It>
		&& requires { typename _Iter_concept<_It>; }
	&& derived_from<_Iter_concept<_It>, input_iterator_tag>;

	template <class _It, class _Ty>
	concept output_iterator = input_or_output_iterator<_It> && indirectly_writable<_It, _Ty>
		&& requires(_It __i, _Ty && __t) {
		*__i++ = static_cast<_Ty&&>(__t);
	};

	template <class _It>
	concept forward_iterator = input_iterator<_It> && derived_from<_Iter_concept<_It>, forward_iterator_tag>
		&& incrementable<_It> && sentinel_for<_It, _It>;

	template <class _It>
	concept bidirectional_iterator = forward_iterator<_It> && derived_from<_Iter_concept<_It>, bidirectional_iterator_tag>
		&& requires(_It __i) {
			{ --__i } -> same_as<_It&>;
			{ __i-- } -> same_as<_It>;
	};

	template <class _It>
	concept random_access_iterator = bidirectional_iterator<_It>
		&& derived_from<_Iter_concept<_It>, random_access_iterator_tag> && totally_ordered<_It>
		&& sized_sentinel_for<_It, _It> && requires(_It __i, const _It __j, const iter_difference_t<_It> __n) {
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
		&& requires(const _It & __i) {
			{ _STD to_address(__i) } -> same_as<add_pointer_t<iter_reference_t<_It>>>;
	};*/

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
}