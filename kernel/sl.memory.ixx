export module sl.memory;
import sl.typetraits;
import sl.concepts;
import sl.type;
import sl.bit;
import sl.byte;
import sl.utility;
import sl.algorithm;
import sl.numeric_limits;
export namespace std
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
	struct _Get_pointer_type
	{
		using type = typename T::value_type*;
	};

	template <class T>
	struct _Get_pointer_type<T, void_t<typename T::pointer>>
	{
		using type = typename T::pointer;
	};

	template <class T, class = void>
	struct _Get_const_pointer_type
	{
		using _Ptrty = typename _Get_pointer_type<T>::type;
		using _Valty = typename T::value_type;
		using type = typename pointer_traits<_Ptrty>::template rebind<const _Valty>;
	};

	template <class T>
	struct _Get_const_pointer_type<T, void_t<typename T::const_pointer>>
	{
		using type = typename T::const_pointer;
	};

	template <class T, class = void>
	struct _Get_void_pointer_type
	{
		using _Ptrty = typename _Get_pointer_type<T>::type;
		using type = typename pointer_traits<_Ptrty>::template rebind<void>;
	};

	template <class T>
	struct _Get_void_pointer_type<T, void_t<typename T::void_pointer>>
	{
		using type = typename T::void_pointer;
	};

	template <class T, class = void>
	struct _Get_const_void_pointer_type
	{
		using _Ptrty = typename _Get_pointer_type<T>::type;
		using type = typename pointer_traits<_Ptrty>::template rebind<const void>;
	};

	template <class T>
	struct _Get_const_void_pointer_type<T, void_t<typename T::const_void_pointer>>
	{
		using type = typename T::const_void_pointer;
	};

	template <class T, class = void>
	struct _Get_difference_type
	{
		using _Ptrty = typename _Get_pointer_type<T>::type;
		using type = typename pointer_traits<_Ptrty>::difference_type;
	};

	template <class T>
	struct _Get_difference_type<T, void_t<typename T::difference_type>>
	{
		using type = typename T::difference_type;
	};

	template <class T, class = void>
	struct _Get_size_type
	{
		using type = make_unsigned_t<typename _Get_difference_type<T>::type>;
	};

	template <class T>
	struct _Get_size_type<T, void_t<typename T::size_type>>
	{
		using type = typename T::size_type;
	};

	template <class T, class = void>
	struct _Get_propagate_on_container_copy
	{
		using type = false_type;
	};

	template <class T>
	struct _Get_propagate_on_container_copy<T, void_t<typename T::propagate_on_container_copy_assignment>>
	{
		using type = typename T::propagate_on_container_copy_assignment;
	};

	template <class T, class = void>
	struct _Get_propagate_on_container_move
	{
		using type = false_type;
	};

	template <class T>
	struct _Get_propagate_on_container_move<T, void_t<typename T::propagate_on_container_move_assignment>>
	{
		using type = typename T::propagate_on_container_move_assignment;
	};

	template <class T, class = void>
	struct _Get_propagate_on_container_swap
	{
		using type = false_type;
	};

	template <class T>
	struct _Get_propagate_on_container_swap<T, void_t<typename T::propagate_on_container_swap>>
	{
		using type = typename T::propagate_on_container_swap;
	};

	template <class T, class = void>
	struct _Get_is_always_equal
	{
		using type = bool_constant<is_empty_v<T>>;
	};

	template <class T>
	struct _Get_is_always_equal<T, void_t<typename T::is_always_equal>>
	{
		using type = typename T::is_always_equal;
	};

	template <class T, class U, class = void>
	struct _Get_rebind_type
	{
		using type = typename _Replace_first_parameter<U, T>::type;
	};

	template <class T, class U>
	struct _Get_rebind_type<T, U, void_t<typename T::template rebind<U>::other>>
	{
		using type = typename T::template rebind<U>::other;
	};

	template <class T>
	class allocator;

	template <class T, class = void>
	struct _Is_default_allocator : false_type {};

	template <class T>
	struct _Is_default_allocator<allocator<T>, void_t<typename allocator<T>::_From_primary>>
		: is_same<typename allocator<T>::_From_primary, allocator<T>>::type {};

	template <class Void, class... _Types>
	struct _Has_no_allocator_construct : true_type {};

	template <class T, class _Ptr, class... _Args>
	struct _Has_no_allocator_construct<
		void_t<decltype(std::declval<T&>().construct(std::declval<_Ptr>(), std::declval<_Args>()...))>, T, _Ptr, _Args...> : false_type {};

	struct _Default_allocate_traits
	{
		__declspec(allocator) static void* _Allocate(const size_t _Bytes)
		{
			return ::operator new(_Bytes);
		}

		__declspec(allocator) static void* _Allocate_aligned(const size_t _Bytes, const size_t _Align)
		{
			return ::operator new(_Bytes, align_val_t{ _Align });
		}
	};

	inline constexpr size_t _Big_allocation_threshold = 4096;
	inline constexpr size_t _Big_allocation_alignment = 32;
#ifdef _DEBUG
	inline constexpr size_t _Non_user_size = 2 * sizeof(void*) + _Big_allocation_alignment - 1;
#else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
	inline constexpr size_t _Non_user_size = sizeof(void*) + _Big_allocation_alignment - 1;
#endif // ^^^ !defined(_DEBUG) ^^^

	template <class _Traits>
	__declspec(allocator) void* _Allocate_manually_vector_aligned(const size_t _Bytes) {
		// allocate _Bytes manually aligned to at least _Big_allocation_alignment
		const size_t _Block_size = _Non_user_size + _Bytes;
		if (_Block_size <= _Bytes) {
			//_Throw_bad_array_new_length(); // add overflow
		}

		const uint64_t _Ptr_container = reinterpret_cast<uint64_t>(_Traits::_Allocate(_Block_size));
		void* const _Ptr = reinterpret_cast<void*>((_Ptr_container + _Non_user_size) & ~(_Big_allocation_alignment - 1));
		static_cast<uint64_t*>(_Ptr)[-1] = _Ptr_container;

#ifdef _DEBUG
		static_cast<uint64_t*>(_Ptr)[-2] = _Big_allocation_sentinel;
#endif // defined(_DEBUG)
		return _Ptr;
	}

	inline void _Adjust_manually_vector_aligned(void*& _Ptr, size_t& _Bytes) {
		// adjust parameters from _Allocate_manually_vector_aligned to pass to operator delete
		_Bytes += _Non_user_size;

		const uint64_t* const _Ptr_user = static_cast<uint64_t*>(_Ptr);
		const uint64_t _Ptr_container = _Ptr_user[-1];

		// If the following asserts, it likely means that we are performing
		// an aligned delete on memory coming from an unaligned allocation.
		//static_assert(_Ptr_user[-2] == _Big_allocation_sentinel, "invalid argument");

		// Extra paranoia on aligned allocation/deallocation; ensure _Ptr_container is
		// in range [_Min_back_shift, _Non_user_size]
#ifdef _DEBUG
		constexpr uintptr_t _Min_back_shift = 2 * sizeof(void*);
#else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
		constexpr uint64_t _Min_back_shift = sizeof(void*);
#endif // ^^^ !defined(_DEBUG) ^^^
		const uint64_t _Back_shift = reinterpret_cast<uint64_t>(_Ptr) - _Ptr_container;
		//_STL_VERIFY(_Back_shift >= _Min_back_shift && _Back_shift <= _Non_user_size, "invalid argument");
		_Ptr = reinterpret_cast<void*>(_Ptr_container);
	}

	template <size_t _Align, class _Traits = _Default_allocate_traits>
	__declspec(allocator) constexpr void* _Allocate(const size_t _Bytes) {
		// allocate _Bytes
		if (_Bytes == 0) {
			return nullptr;
		}

		if (std::is_constant_evaluated()) {
			return _Traits::_Allocate(_Bytes);
		}

		if constexpr (_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
			size_t _Passed_align = _Align;
			if (_Bytes >= _Big_allocation_threshold) {
				// boost the alignment of big allocations to help autovectorization
				_Passed_align = (std::max)(_Align, _Big_allocation_alignment);
			}
			return _Traits::_Allocate_aligned(_Bytes, _Passed_align);
		}
		else
		{
			if (_Bytes >= _Big_allocation_threshold) {
				// boost the alignment of big allocations to help autovectorization
				return _Allocate_manually_vector_aligned<_Traits>(_Bytes);
			}
			return _Traits::_Allocate(_Bytes);
		}
	}

	template <size_t _Align>
	constexpr void _Deallocate(void* _Ptr, size_t _Bytes) noexcept {
		// deallocate storage allocated by _Allocate
		if (std::is_constant_evaluated()) {
			::operator delete(_Ptr);
			return;
		}

		if constexpr (_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
			size_t _Passed_align = _Align;
			if (_Bytes >= _Big_allocation_threshold) {
				// boost the alignment of big allocations to help autovectorization
				_Passed_align = (std::max)(_Align, _Big_allocation_alignment);
			}
			::operator delete(_Ptr, _Bytes, align_val_t{ _Passed_align });
		}
		else
		{
			if (_Bytes >= _Big_allocation_threshold) {
				// boost the alignment of big allocations to help autovectorization
				_Adjust_manually_vector_aligned(_Ptr, _Bytes);
			}
			::operator delete(_Ptr, _Bytes);
		}
	}

	template <size_t _Ty_size>
	[[nodiscard]] constexpr size_t _Get_size_of_n(const size_t _Count) {
		constexpr bool _Overflow_is_possible = _Ty_size > 1;

		if constexpr (_Overflow_is_possible) {
			constexpr size_t _Max_possible = static_cast<size_t>(-1) / _Ty_size;
			if (_Count > _Max_possible) {
				//_Throw_bad_array_new_length(); // multiply overflow
			}
		}

		return _Count * _Ty_size;
	}

	template <class T>
	inline constexpr size_t _New_alignof = (std::max)(alignof(T), __STDCPP_DEFAULT_NEW_ALIGNMENT__);

	template <class _NoThrowFwdIt, class _NoThrowSentinel>
	constexpr void _Destroy_range(_NoThrowFwdIt _First, _NoThrowSentinel _Last) noexcept;

	template <class T>
	constexpr void _Destroy_in_place(T& _Obj) noexcept {
		if constexpr (is_array_v<T>) {
			std::_Destroy_range(_Obj, _Obj + extent_v<T>);
		}
		else {
			_Obj.~T();
		}
	}

	template <class T>
	struct allocator_traits;

	template <class Alloc, class _Ptr, class = void>
	struct _Has_no_alloc_destroy : true_type {};

	template <class Alloc, class _Ptr>
	struct _Has_no_alloc_destroy<Alloc, _Ptr, void_t<decltype(std::declval<Alloc&>().destroy(std::declval<_Ptr>()))>> : false_type {};

	template <class Alloc, class _Ptr>
	using _Uses_default_destroy = disjunction<_Is_default_allocator<Alloc>, _Has_no_alloc_destroy<Alloc, _Ptr>>;

	template <class Alloc>
	using _Alloc_ptr_t = typename allocator_traits<Alloc>::pointer;

	template <class Alloc>
	using _Alloc_size_t = typename allocator_traits<Alloc>::size_type;

	export template <class Alloc>
		constexpr void _Destroy_range(_Alloc_ptr_t<Alloc> _First, const _Alloc_ptr_t<Alloc> _Last, Alloc& _Al) noexcept {
		// note that this is an optimization for debug mode codegen; in release mode the BE removes all of this
		using T = typename Alloc::value_type;
		if constexpr (!conjunction_v<is_trivially_destructible<T>, _Uses_default_destroy<Alloc, T*>>) {
			for (; _First != _Last; ++_First) {
				allocator_traits<Alloc>::destroy(_Al, _First);
			}
		}
	}

	export template <class _NoThrowFwdIt, class _NoThrowSentinel>
		constexpr void _Destroy_range(_NoThrowFwdIt _First, const _NoThrowSentinel _Last) noexcept {
		// note that this is an optimization for debug mode codegen; in release mode the BE removes all of this
		if constexpr (!is_trivially_destructible_v<_Iter_value_t<_NoThrowFwdIt>>) {
			for (; _First != _Last; ++_First) {
				std::_Destroy_in_place(*_First);
			}
		}
	}

	export template <class T> constexpr void destroy_at(T* const _Location) noexcept
	{
		if constexpr (is_array_v<T>) {
			std::_Destroy_range(_Location, _Location);
		}
		else
		{
			_Location->~T();
		}
	}

	template <class Alloc, class _SizeTy>
	concept _Has_member_allocate_at_least = requires(Alloc& _Al, const _SizeTy& _Count) { _Al.allocate_at_least(_Count); };

	template <class Alloc, class _Ptr, class... _Args>
	using _Uses_default_construct = disjunction<_Is_default_allocator<Alloc>, _Has_no_allocator_construct<void, Alloc, _Ptr, _Args...>>;

	template <class Alloc, class _Size_type, class _Const_void_pointer, class = void>
	struct _Has_allocate_hint : false_type {};

	template <class Alloc, class _Size_type, class _Const_void_pointer>
	struct _Has_allocate_hint<Alloc, _Size_type, _Const_void_pointer, void_t<decltype(std::declval<Alloc&>().allocate(std::declval<const _Size_type&>(), std::declval<const _Const_void_pointer&>()))>> : true_type {};

	template <class Alloc, class = void>
	struct _Has_max_size : false_type {};

	template <class Alloc>
	struct _Has_max_size<Alloc, void_t<decltype(std::declval<const Alloc&>().max_size())>> : true_type {};

	template <class Alloc, class = void>
	struct _Has_select_on_container_copy_construction : false_type {};

	template <class Alloc>
	struct _Has_select_on_container_copy_construction<Alloc, void_t<decltype(std::declval<const Alloc&>().select_on_container_copy_construction())>> : true_type {};

	template <class _Ptr, class _SizeTy = size_t>
	struct allocation_result {
		_Ptr ptr;
		_SizeTy count;
	};

	export template <class T>
	struct _Default_allocator_traits
	{
		using allocator_type = T;
		using value_type = typename T::value_type;

		using pointer = value_type*;
		using const_pointer = const value_type*;
		using void_pointer = void*;
		using const_void_pointer = const void*;

		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using propagate_on_container_copy_assignment = false_type;
		using propagate_on_container_move_assignment = true_type;
		using propagate_on_container_swap = false_type;
		using is_always_equal = true_type;

		template <class U>
		using rebind_alloc = allocator<U>;

		template <class U>
		using rebind_traits = allocator_traits<allocator<U>>;

		[[nodiscard]] static constexpr __declspec(allocator) pointer allocate(T& alloc, const size_type _Count)
		{
			if (std::is_constant_evaluated()) {
				return alloc.allocate(_Count);
			}
			else
			{
				(void)alloc;
				return static_cast<pointer>(_Allocate<_New_alignof<value_type>>(_Get_size_of_n<sizeof(value_type)>(_Count)));
			}
		}

		[[nodiscard]] static constexpr __declspec(allocator) pointer allocate(T& alloc, const size_type _Count, const_void_pointer)
		{
			if (std::is_constant_evaluated()) {
				return alloc.allocate(_Count);
			}
			else
			{
				(void)alloc;
				return static_cast<pointer>(_Allocate<_New_alignof<value_type>>(_Get_size_of_n<sizeof(value_type)>(_Count)));
			}
		}

		[[nodiscard]] static constexpr allocation_result<pointer, size_type> allocate_at_least(T& alloc, const size_type _Count) {
			return { alloc.allocate(_Count), _Count };
		}

		static constexpr void deallocate(T& alloc, const pointer _Ptr, const size_type _Count) {
			// no overflow check on the following multiply; we assume _Allocate did that check
			if (std::is_constant_evaluated()) {
				alloc.deallocate(_Ptr, _Count);
			}
			else
			{
				(void)alloc;
				std::_Deallocate<_New_alignof<value_type>>(_Ptr, sizeof(value_type) * _Count);
			}
		}

		template <class _Objty, class... _Types>
		static constexpr void construct(T&, _Objty* const _Ptr, _Types&&... _Args) {
			std::construct_at(_Ptr, std::forward<_Types>(_Args)...);
		}

		template <class _Uty>
		static constexpr void destroy(T&, _Uty* const _Ptr)
		{
			std::destroy_at(_Ptr);
		}

		[[nodiscard]] static constexpr size_type max_size(const T&) noexcept {
			return static_cast<size_t>(-1) / sizeof(value_type);
		}

		[[nodiscard]] static constexpr T select_on_container_copy_construction(const T& alloc) {
			return alloc;
		}
	};


	template <class T>
	struct _Normal_allocator_traits
	{
		using allocator_type = T;
		using value_type = typename T::value_type;

		using pointer = typename _Get_pointer_type<T>::type;
		using const_pointer = typename _Get_const_pointer_type<T>::type;
		using void_pointer = typename _Get_void_pointer_type<T>::type;
		using const_void_pointer = typename _Get_const_void_pointer_type<T>::type;

		using size_type = typename _Get_size_type<T>::type;
		using difference_type = typename _Get_difference_type<T>::type;

		using propagate_on_container_copy_assignment = typename _Get_propagate_on_container_copy<T>::type;
		using propagate_on_container_move_assignment = typename _Get_propagate_on_container_move<T>::type;
		using propagate_on_container_swap = typename _Get_propagate_on_container_swap<T>::type;
		using is_always_equal = typename _Get_is_always_equal<T>::type;

		template <class U>
		using rebind_alloc = typename _Get_rebind_type<T, U>::type;

		template <class U>
		using rebind_traits = allocator_traits<rebind_alloc<U>>;

		[[nodiscard]] static constexpr __declspec(allocator) pointer allocate(T& alloc, const size_type _Count)
		{
			return alloc.allocate(_Count);
		}

		[[nodiscard]] static constexpr __declspec(allocator) pointer allocate(T& alloc, const size_type _Count, const const_void_pointer _Hint)
		{
			if constexpr (_Has_allocate_hint<T, size_type, const_void_pointer>::value)
				return alloc.allocate(_Count, _Hint);
			else
				return alloc.allocate(_Count);
		}

		[[nodiscard]] static constexpr allocation_result<pointer, size_type> allocate_at_least(T& alloc, const size_type _Count) {
			if constexpr (_Has_member_allocate_at_least<T, size_type>)
				return alloc.allocate_at_least(_Count);
			else
				return { alloc.allocate(_Count), _Count };
		}

		static constexpr void deallocate(T& alloc, pointer ptr, size_type count)
		{
			alloc.deallocate(ptr, count);
		}

		template <class _Objty, class... _Types>
		static constexpr void construct(T& alloc, _Objty* const _Ptr, _Types&&... _Args)
		{
			if constexpr (_Uses_default_construct<T, _Objty*, _Types...>::value)
				std::construct_at(_Ptr, std::forward<_Types>(_Args)...);
			else
				alloc.construct(_Ptr, std::forward<_Types>(_Args)...);
		}

		template <class _Uty>
		static constexpr void destroy(T& alloc, _Uty* _Ptr)
		{
			if constexpr (_Uses_default_destroy<T, _Uty*>::value) {
				std::destroy_at(_Ptr);
			}
			else {
				alloc.destroy(_Ptr);
			}
		}

		[[nodiscard]] static constexpr size_type max_size(const T& alloc) noexcept
		{
			if constexpr (_Has_max_size<T>::value)
				return alloc.max_size();
			else
				return (numeric_limits<size_type>::max)() / sizeof(value_type);
		}

		[[nodiscard]] static constexpr T select_on_container_copy_construction(const T& alloc) {
			if constexpr (_Has_select_on_container_copy_construction<T>::value) {
				return alloc.select_on_container_copy_construction();
			}
			else {
				return alloc;
			}
		}
	};

	template <class _Alloc, class _Value_type>
	using _Rebind_alloc_t = typename allocator_traits<_Alloc>::template rebind_alloc<_Value_type>;

	template <class Alloc>
	class [[nodiscard]] _Uninitialized_backout_al
	{
	private:
		using pointer = _Alloc_ptr_t<Alloc>;

	public:
		constexpr _Uninitialized_backout_al(pointer _Dest, Alloc& _Al_) : _First(_Dest), _Last(_Dest), _Al(_Al_) {}

		_Uninitialized_backout_al(const _Uninitialized_backout_al&) = delete;
		_Uninitialized_backout_al& operator=(const _Uninitialized_backout_al&) = delete;

		constexpr ~_Uninitialized_backout_al()
		{
			std::_Destroy_range(_First, _Last, _Al);
		}

		template <class... T>
		constexpr void _Emplace_back(T&&... _Vals)
		{
			allocator_traits<Alloc>::construct(_Al, _Last, std::forward<T>(_Vals)...);
			++_Last;
		}

		constexpr pointer _Release()
		{
			_First = _Last;
			return _Last;
		}

	private:
		pointer _First;
		pointer _Last;
		Alloc& _Al;
	};
}

export namespace std
{
	template <class T, class _Ptr, class... _Args>
	using _Uses_default_construct =
		disjunction<_Is_default_allocator<T>, _Has_no_allocator_construct<void, T, _Ptr, _Args...>>;

	template <class T>
	struct allocator_traits : conditional_t<_Is_default_allocator<T>::value, _Default_allocator_traits<T>,
		_Normal_allocator_traits<T>> {};

	template <class T>
	[[nodiscard]] constexpr T* addressof(T& value) noexcept
	{
		return __builtin_addressof(value);
	}

	template <class T>
	const T* addressof(const T&&) = delete;

	void set(void* p, uint8_t value, size_t size);
	void copy(void* destination, const void* source, size_t size);
	void move(void* destination, const void* source, size_t size);

	template <class T>
	struct _Is_character : false_type {}; // by default, not a character type

	template <>
	struct _Is_character<char> : true_type {}; // chars are characters

	template <>
	struct _Is_character<signed char> : true_type {}; // signed chars are also characters

	template <>
	struct _Is_character<unsigned char> : true_type {}; // unsigned chars are also characters

#ifdef __cpp_char8_t
	template <>
	struct _Is_character<char8_t> : true_type {}; // UTF-8 code units are sort-of characters
#endif // defined(__cpp_char8_t)

	template <class T>
	struct _Is_character_or_bool : _Is_character<T>::type {};

	template <>
	struct _Is_character_or_bool<bool> : true_type {};

	template <class T>
	struct _Is_character_or_byte_or_bool : _Is_character_or_bool<T>::type {};

	template <>
	struct _Is_character_or_byte_or_bool<byte> : true_type {};

	// _Fill_memset_is_safe determines if _FwdIt and T are eligible for memset optimization in fill.
	// Need to explicitly test for volatile because _Unwrap_enum_t discards qualifiers.
	template <class _FwdIt, class T, bool = _Iterator_is_contiguous<_FwdIt>>
	constexpr bool _Fill_memset_is_safe = conjunction_v<is_scalar<T>,
		_Is_character_or_byte_or_bool<_Unwrap_enum_t<remove_reference_t<_Iter_ref_t<_FwdIt>>>>,
		negation<is_volatile<remove_reference_t<_Iter_ref_t<_FwdIt>>>>, is_assignable<_Iter_ref_t<_FwdIt>, const T&>>;

	template <class _FwdIt, class T>
	constexpr bool _Fill_memset_is_safe<_FwdIt, T, false> = false;

	template <class _FwdIt, class T, bool = _Iterator_is_contiguous<_FwdIt>>
	constexpr bool _Fill_zero_memset_is_safe =
		conjunction_v<is_scalar<T>, is_scalar<_Iter_value_t<_FwdIt>>, negation<is_member_pointer<_Iter_value_t<_FwdIt>>>,
		negation<is_volatile<remove_reference_t<_Iter_ref_t<_FwdIt>>>>, is_assignable<_Iter_ref_t<_FwdIt>, const T&>>;

	template <class _FwdIt, class T>
	constexpr bool _Fill_zero_memset_is_safe<_FwdIt, T, false> = false;

	template <class _CtgIt, class T>
	void _Fill_memset(_CtgIt _Dest, const T _Val, const size_t _Count) {
		// implicitly convert (a cast would suppress warnings); also handles _Iter_value_t<_CtgIt> being bool
		_Iter_value_t<_CtgIt> _Dest_val = _Val;
		::memset(std::_To_address(_Dest), static_cast<unsigned char>(_Dest_val), _Count);
	}

	template <class _CtgIt>
	void _Fill_zero_memset(_CtgIt _Dest, const size_t _Count) {
		::memset(std::_To_address(_Dest), 0, _Count * sizeof(_Iter_value_t<_CtgIt>));
	}

	template <class T>
	[[nodiscard]] bool _Is_all_bits_zero(const T& _Val) {
		if constexpr (is_same_v<T, nullptr_t>) {
			return true;
		}
		else {
			constexpr T _Zero{};
			return ::memcmp(&_Val, &_Zero, sizeof(T)) == 0;
		}
	}

	template <class _Alloc>
	constexpr _Alloc_ptr_t<_Alloc> _Uninitialized_fill_n(
		_Alloc_ptr_t<_Alloc> _First, _Alloc_size_t<_Alloc> _Count, const typename _Alloc::value_type& _Val, _Alloc& _Al) {
		// copy _Count copies of _Val to raw _First, using _Al
		using T = typename _Alloc::value_type;
		if constexpr (_Fill_memset_is_safe<T*, T> && _Uses_default_construct<_Alloc, T*, T>::value) {
			if (!std::is_constant_evaluated())
			{
				_Fill_memset(_First, _Val, static_cast<size_t>(_Count));
				return _First + _Count;
			}
		}
		else if constexpr (_Fill_zero_memset_is_safe<T*, T> && _Uses_default_construct<_Alloc, T*, T>::value) {
			if (!std::is_constant_evaluated())
			{
				if (_Is_all_bits_zero(_Val)) {
					_Fill_zero_memset(_First, static_cast<size_t>(_Count));
					return _First + _Count;
				}
			}
		}

		_Uninitialized_backout_al<_Alloc> _Backout{ _First, _Al };
		for (; 0 < _Count; --_Count) {
			_Backout._Emplace_back(_Val);
		}

		return _Backout._Release();
	}

	//template <class _NoThrowFwdIt, class _Tval>
	//	void uninitialized_fill(const _NoThrowFwdIt _First, const _NoThrowFwdIt _Last, const _Tval& _Val) {
	//	// copy _Val throughout raw [_First, _Last)
	//	std::_Adl_verify_range(_First, _Last);
	//	auto _UFirst = _First;
	//	const auto _ULast = _Last;
	//	if constexpr (_Fill_memset_is_safe<_Unwrapped_t<const _NoThrowFwdIt&>, _Tval>) {
	//		std::_Fill_memset(_UFirst, _Val, static_cast<size_t>(_ULast - _UFirst));
	//	}
	//	else {
	//		if constexpr (_Fill_zero_memset_is_safe<_Unwrapped_t<const _NoThrowFwdIt&>, _Tval>) {
	//			if (std::_Is_all_bits_zero(_Val)) {
	//				std::_Fill_zero_memset(_UFirst, static_cast<size_t>(_ULast - _UFirst));
	//				return;
	//			}
	//		}

	//		_Uninitialized_backout<_Unwrapped_t<const _NoThrowFwdIt&>> _Backout{ _UFirst };
	//		while (_Backout._Last != _ULast) {
	//			_Backout._Emplace_back(_Val);
	//		}

	//		_Backout._Release();
	//	}
	//}

	template <class _NoThrowFwdIt>
	constexpr bool _Use_memset_value_construct_v = conjunction_v<bool_constant<_Iterator_is_contiguous<_NoThrowFwdIt>>,
		is_scalar<_Iter_value_t<_NoThrowFwdIt>>, negation<is_volatile<remove_reference_t<_Iter_ref_t<_NoThrowFwdIt>>>>,
		negation<is_member_pointer<_Iter_value_t<_NoThrowFwdIt>>>>;

	template <class _Ptr>
	_Ptr _Zero_range(const _Ptr _First, const _Ptr _Last) { // fill [_First, _Last) with zeroes
		char* const _First_ch = reinterpret_cast<char*>(std::_To_address(_First));
		char* const _Last_ch = reinterpret_cast<char*>(std::_To_address(_Last));
		::memset(_First_ch, 0, static_cast<size_t>(_Last_ch - _First_ch));
		return _Last;
	}

	template <class _Alloc>
	constexpr _Alloc_ptr_t<_Alloc> _Uninitialized_value_construct_n(
		_Alloc_ptr_t<_Alloc> _First, _Alloc_size_t<_Alloc> _Count, _Alloc& _Al) {
		// value-initialize _Count objects to raw _First, using _Al
		using _Ptrty = typename _Alloc::value_type*;
		if constexpr (_Use_memset_value_construct_v<_Ptrty> && _Uses_default_construct<_Alloc, _Ptrty>::value) {
			if (!std::is_constant_evaluated())
			{
				auto _PFirst = _First;
				_Zero_range(_PFirst, _PFirst + _Count);
				return _First + _Count;
			}
		}

		_Uninitialized_backout_al<_Alloc> _Backout{ _First, _Al };
		for (; 0 < _Count; --_Count) {
			_Backout._Emplace_back();
		}

		return _Backout._Release();
	}

	template <class _InIt, class _Se, class Alloc>
	constexpr _Alloc_ptr_t<Alloc> _Uninitialized_copy(
		_InIt _First, _Se _Last, _Alloc_ptr_t<Alloc> _Dest, Alloc& _Al) {
		// copy [_First, _Last) to raw _Dest, using _Al
		// note: only called internally from elsewhere in the STL
		using _Ptrval = typename Alloc::value_type*;
		_InIt _UFirst = _First;
		const _InIt _ULast = _Last;
		
		constexpr bool _Can_memmove = _Sent_copy_cat<decltype(_UFirst), decltype(_ULast), _Ptrval>::_Bitcopy_constructible
			&& _Uses_default_construct<Alloc, _Ptrval, decltype(*_UFirst)>::value;

		if constexpr (_Can_memmove) {
			if (!std::is_constant_evaluated())
			{
				if constexpr (is_same_v<decltype(_UFirst), decltype(_ULast)>) {
					std::_Copy_memmove(std::_To_address(_UFirst), std::_To_address(_ULast), _Dest);
					_Dest += _ULast - _UFirst;
				}
				else {
					const auto _Count = static_cast<size_t>(_ULast - _UFirst);
					std::_Copy_memmove_n(std::_To_address(_UFirst), _Count, _Dest);
					_Dest += _Count;
				}
				return _Dest;
			}
		}

		_Uninitialized_backout_al<Alloc> _Backout{ _Dest, _Al };
		for (; _UFirst != _ULast; ++_UFirst) {
			_Backout._Emplace_back(*_UFirst);
		}

		return _Backout._Release();
	}

	template <class _InIt, class Alloc>
	constexpr _Alloc_ptr_t<Alloc> _Uninitialized_move(
		const _InIt _First, const _InIt _Last, _Alloc_ptr_t<Alloc> _Dest, Alloc& _Al) {
		// move [_First, _Last) to raw _Dest, using _Al
		// note: only called internally from elsewhere in the STL
		_InIt _UFirst = _First;
		const _InIt _ULast = _Last;
		using _Ptrval = typename Alloc::value_type*;
		if constexpr (conjunction_v<bool_constant<_Iter_move_cat<decltype(_UFirst), _Ptrval>::_Bitcopy_constructible>,
			_Uses_default_construct<Alloc, _Ptrval, decltype(std::move(*_UFirst))>>) {
			if constexpr (!std::is_constant_evaluated())
			{
				_Copy_memmove(_UFirst, _ULast, _Dest);
				return _Dest + (_UFirst - _ULast);
			}
		}

		_Uninitialized_backout_al<Alloc> _Backout{ _Dest, _Al };
		for (; _UFirst != _ULast; ++_UFirst) {
			_Backout._Emplace_back(std::move(*_UFirst));
		}

		return _Backout._Release();
	}
	export template <class T>
	class allocator
	{
	public:
		static_assert(!is_const_v<T>, "The C++ Standard forbids containers of const elements "
			"because allocator<const T> is ill-formed.");
		static_assert(!is_function_v<T>, "The C++ Standard forbids allocators for function elements "
			"because of [allocator.requirements].");
		static_assert(!is_reference_v<T>, "The C++ Standard forbids allocators for reference elements "
			"because of [allocator.requirements].");
		using _From_primary = allocator;
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = true_type;

		constexpr allocator() noexcept {}

		constexpr allocator(const allocator&) noexcept = default;
		template <class U>
		constexpr allocator(const allocator<U>& other) noexcept {}
		constexpr ~allocator() = default;
		constexpr allocator& operator=(const allocator&) = default;

		[[nodiscard]] constexpr __declspec(allocator) T* allocate(const size_t n) const
		{
			static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
			return static_cast<T*>(std::_Allocate<std::_New_alignof<T>>(std::_Get_size_of_n<sizeof(T)>(n)));
		}
		//_CONSTEXPR20 void deallocate(T* const _Ptr, const size_t _Count) {
		//	_STL_ASSERT(_Ptr != nullptr || _Count == 0, "null pointer cannot point to a block of non-zero size");
		//	// no overflow check on the following multiply; we assume _Allocate did that check
		//	std::_Deallocate<_New_alignof<T>>(_Ptr, sizeof(T) * _Count);
		//}
		constexpr void deallocate(T* const p, size_t n) const
		{
			std::_Deallocate<std::_New_alignof<T>>(p, sizeof(T) * n);
		}
		[[nodiscard]] constexpr allocation_result<T*> allocate_at_least(const size_t n)
		{
			return { allocate(n), n };
		}
	};
	template <class T1, class T2>
	constexpr bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept
	{
		return lhs == rhs;
	}
}
export
{
	[[nodiscard]] int memcmp(void const* buf1, void const* buf2, size_t size);
}