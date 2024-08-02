export module sl.memory;
import sl.typetraits;
import sl.concepts;
import sl.allocator;
import sl.type;
import sl.utility;
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

	template <class T, class = void>
	struct _Is_default_allocator : false_type {};

	template <class T>
	struct _Is_default_allocator<allocator<T>, void_t<typename allocator<T>::_From_primary>>
		: is_same<typename allocator<T>::_From_primary, allocator<T>>::type {};

	template <class _Void, class... _Types>
	struct _Has_no_allocator_construct : true_type {};

	template <class T, class _Ptr, class... _Args>
	struct _Has_no_allocator_construct<
		void_t<decltype(std::declval<T&>().construct(std::declval<_Ptr>(), std::declval<_Args>()...))>, T, _Ptr, _Args...> : false_type {};

	
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
				_Passed_align = (_STD max)(_Align, _Big_allocation_alignment);
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
				_Passed_align = (_STD max)(_Align, _Big_allocation_alignment);
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
				_Throw_bad_array_new_length(); // multiply overflow
			}
		}

		return _Count * _Ty_size;
	}

	template <class T>
	inline constexpr size_t _New_alignof = (std::max)(alignof(T), __STDCPP_DEFAULT_NEW_ALIGNMENT__);


	template <class T>
	struct allocator_traits;

	template <class T>
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

		[[nodiscard]] static constexpr allocation_result<pointer, size_type> allocate_at_least(T& alloc,  const size_type _Count) {
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
		static constexpr void destroy(T&, _Uty* const _Ptr) {
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

	template <class _Ptrty>
	[[nodiscard]] constexpr auto _Unfancy(_Ptrty _Ptr) noexcept
	{
		return addressof(*_Ptr);
	}

	template <class T>
	[[nodiscard]] constexpr T* _Unfancy(T* _Ptr) noexcept
	{
		return _Ptr;
	}

	void set(void* p, uint8_t value, size_t size);
	void copy(void* destination, const void* source, size_t size);
	void move(void* destination, const void* source, size_t size);
}
export
{
	[[nodiscard]] int memcmp(void const* buf1, void const* buf2, size_t size);
}