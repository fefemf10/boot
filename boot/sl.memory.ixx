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

	template <class _Alloc, class _Ptr, class... _Args>
	struct _Has_no_allocator_construct<
		void_t<decltype(std::declval<_Alloc&>().construct(std::declval<_Ptr>(), std::declval<_Args>()...))>, _Alloc, _Ptr, _Args...> : false_type {};

	

	template <class _Alloc>
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
	template <class _Alloc, class _Ptr, class... _Args>
	using _Uses_default_construct =
		disjunction<_Is_default_allocator<_Alloc>, _Has_no_allocator_construct<void, _Alloc, _Ptr, _Args...>>;

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
}