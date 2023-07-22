export module sl.typetraits;

export namespace std
{
	template <class T, T value>
	struct integral_constant
	{
		static constexpr T value = value;
		using type = integral_constant;

		constexpr operator T() const noexcept { return value; }
		[[nodiscard]] constexpr T operator()() const noexcept { return value; }
	};

	template <bool value> using bool_constant = integral_constant<bool, value>;
	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template <bool Test, class T = void> struct enable_if {};
	template <class T> struct enable_if<true, T> { using type = T; };
	template <bool Test, class T = void> using enable_if_t = typename enable_if<Test, T>::type;

	template <bool Test, class T1, class T2> struct conditional { using type = T1; };
	template <class T1, class T2> struct conditional<false, T1, T2> { using type = T2; };
	template <bool Test, class T1, class T2> using conditional_t = typename conditional<Test, T1, T2>::type;

	template <class T1, class T2> inline constexpr bool is_same_v = false;
	template <class T> inline constexpr bool is_same_v<T, T> = true;
	template <class T1, class T2> struct is_same : bool_constant<is_same_v<T1, T2>> {};

	template <class T> struct remove_const { using type = T; };
	template <class T> struct remove_const<const T> { using type = T; };
	template <class T> using remove_const_t = typename remove_const<T>::type;

	template <class T> struct remove_volatile { using type = T; };
	template <class T> struct remove_volatile<volatile T> { using type = T; };
	template <class T> using remove_volatile_t = typename remove_volatile<T>::type;

	template <class T>
	struct remove_cv
	{
		using type = T;
		template <template <class> class Fn> using Apply = Fn<T>;
	};

	template <class T>
	struct remove_cv<const T>
	{
		using type = T;
		template <template <class> class Fn> using Apply = const Fn<T>;
	};

	template <class T>
	struct remove_cv<volatile T>
	{
		using type = T;
		template <template <class> class Fn> using Apply = volatile Fn<T>;
	};

	template <class T>
	struct remove_cv<const volatile T>
	{
		using type = T;
		template <template <class> class Fn> using Apply = const volatile Fn<T>;
	};

	template <class T> using remove_cv_t = typename remove_cv<T>::type;

	template <bool First_value, class First, class... Rest> struct Disjunction { using type = First; };
	template <class False, class Next, class... Rest> struct Disjunction<false, False, Next, Rest...> { using type = typename Disjunction<Next::value, Next, Rest...>::type; };

	template <class... Traits> struct disjunction : false_type {};
	template <class First, class... Rest> struct disjunction<First, Rest...> : Disjunction<First::value, First, Rest...>::type {};

	template <class... Traits> inline constexpr bool disjunction_v = disjunction<Traits...>::value;

	template <class T, class... Types> inline constexpr bool Is_any_of_v = disjunction_v<is_same<T, Types>...>;

	template <class T> inline constexpr bool is_integral_v = Is_any_of_v<remove_cv_t<T>, bool, char, signed char, unsigned char, char8_t, char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;
	template <class T> struct is_integral : bool_constant<is_integral_v<T>> {};

	template <class T> inline constexpr bool is_floating_point_v = Is_any_of_v<remove_cv_t<T>, float, double, long double>;
	template <class T> struct is_floating_point : bool_constant<is_floating_point_v<T>> {};

	template <class T> inline constexpr bool is_arithmetic_v = is_integral_v<T> || is_floating_point_v<T>;
	template <class T> struct is_arithmetic : bool_constant<is_arithmetic_v<T>> {};

	template <class T> inline constexpr bool is_character_v = Is_any_of_v<remove_cv_t<T>, char, char8_t, char16_t, char32_t>;
	template <class T> struct is_character : bool_constant<is_character_v<T>> {};

	template <class T>
	struct remove_reference {
		using type = T;
		using _Const_thru_ref_type = const T;
	};

	template <class T>
	struct remove_reference<T&> {
		using type = T;
		using _Const_thru_ref_type = const T&;
	};

	template <class T>
	struct remove_reference<T&&> {
		using type = T;
		using _Const_thru_ref_type = const T&&;
	};

	template <class T> using remove_reference_t = typename remove_reference<T>::type;
	template <class T> using _Const_thru_ref = typename remove_reference<T>::_Const_thru_ref_type;
	template <class T> using _Remove_cvref_t [[msvc::known_semantics]] = remove_cv_t<remove_reference_t<T>>;
	template <class T> using remove_cvref_t = _Remove_cvref_t<T>;
	template <class T> struct remove_cvref { using type = remove_cvref_t<T>; };

	template <class T, T... Vals>
	struct integer_sequence { // sequence of integer parameters
		static_assert(is_integral_v<T>, "integer_sequence<T, I...> requires T to be an integral type.");

		using value_type = T;

		[[nodiscard]] static constexpr size_t size() noexcept {
			return sizeof...(Vals);
		}
	};

	template <class T, T Size> using make_integer_sequence = __make_integer_seq<integer_sequence, T, Size>;
	template <size_t... Vals> using index_sequence = integer_sequence<size_t, Vals...>;
	template <size_t Size> using make_index_sequence = make_integer_sequence<size_t, Size>;
	template <class... Types> using index_sequence_for = make_index_sequence<sizeof...(Types)>;

	template <bool _First_value, class _First, class... _Rest> struct _Conjunction { using type = _First; };
	template <class _True, class _Next, class... _Rest> struct _Conjunction<true, _True, _Next, _Rest...> { using type = typename _Conjunction<_Next::value, _Next, _Rest...>::type; };

	template <class... Traits> struct conjunction : true_type {};
	template <class _First, class... _Rest> struct conjunction<_First, _Rest...> : _Conjunction<_First::value, _First, _Rest...>::type {};

	template <class... Traits> inline constexpr bool conjunction_v = conjunction<Traits...>::value;

	template <class Trait> struct negation : bool_constant<!static_cast<bool>(Trait::value)> {};
	template <class Trait> inline constexpr bool negation_v = negation<Trait>::value;

	template <class T> inline constexpr bool is_void_v = is_same_v<remove_cv_t<T>, void>;
	template <class T> struct is_void : bool_constant<is_void_v<T>> {};

	template <class... Types> using void_t = void;
	template <class T>
	struct _Identity { using type = T; };
	template <class T> using _Identity_t [[msvc::known_semantics]] = typename _Identity<T>::type;

	template <class T> struct add_const { using type = const T; };
	template <class T> using add_const_t = typename add_const<T>::type;

	template <class T> struct add_volatile { using type = volatile T; };
	template <class T> using add_volatile_t = typename add_volatile<T>::type;

	template <class T> struct add_cv { using type = const volatile T; };
	template <class T> using add_cv_t = typename add_cv<T>::type;

	template <class T, class = void>
	struct Add_reference
	{
		using _Lvalue = T;
		using _Rvalue = T;
	};

	template <class T>
	struct Add_reference<T, void_t<T&>>
	{
		using _Lvalue = T&;
		using _Rvalue = T&&;
	};
	template <class>
	inline constexpr bool _Always_false = false;

	template <class T> struct add_lvalue_reference { using type = typename Add_reference<T>::_Lvalue; };
	template <class T> using add_lvalue_reference_t = typename Add_reference<T>::_Lvalue;
	template <class T> struct add_rvalue_reference { using type = typename Add_reference<T>::_Rvalue; };
	template <class T> using add_rvalue_reference_t = typename Add_reference<T>::_Rvalue;
	template <class T> add_rvalue_reference_t<T> declval() noexcept { static_assert(_Always_false<T>, "Calling declval is ill-formed, see N4892 [declval]/2."); }

	template <class T> struct remove_extent { using type = T; };
	template <class T, size_t _Ix> struct remove_extent<T[_Ix]> { using type = T; };
	template <class T> struct remove_extent<T[]> { using type = T; };
	template <class T> using remove_extent_t = typename remove_extent<T>::type;
	template <class T> struct remove_all_extents { using type = T; };
	template <class T, size_t _Ix> struct remove_all_extents<T[_Ix]> { using type = typename remove_all_extents<T>::type; };
	template <class T> struct remove_all_extents<T[]> { using type = typename remove_all_extents<T>::type; };
	template <class T> using remove_all_extents_t = typename remove_all_extents<T>::type;

	template <class T> struct remove_pointer { using type = T; };
	template <class T> struct remove_pointer<T*> { using type = T; };
	template <class T> struct remove_pointer<T* const> { using type = T; };
	template <class T> struct remove_pointer<T* volatile> { using type = T; };
	template <class T> struct remove_pointer<T* const volatile> { using type = T; };
	template <class T> using remove_pointer_t = typename remove_pointer<T>::type;

	template <class T, class = void> struct Add_pointer { using type = T; };
	template <class T> struct Add_pointer<T, void_t<remove_reference_t<T>*>> { using type = remove_reference_t<T>*; };
	template <class T> struct add_pointer { using type = typename Add_pointer<T>::type; };
	template <class T> using add_pointer_t = typename Add_pointer<T>::type;

	template <class T> inline constexpr bool is_array_v = false;
	template <class T, size_t size> inline constexpr bool is_array_v<T[size]> = true;
	template <class T> inline constexpr bool is_array_v<T[]> = true;
	template <class T> struct is_array : bool_constant<is_array_v<T>> {};

	template <class T> inline constexpr bool is_bounded_array_v = false;
	template <class T, size_t size> inline constexpr bool is_bounded_array_v<T[size]> = true;
	template <class T> struct is_bounded_array : bool_constant<is_bounded_array_v<T>> {};

	template <class T> inline constexpr bool is_unbounded_array_v = false;
	template <class T> inline constexpr bool is_unbounded_array_v<T[]> = true;
	template <class T> struct is_unbounded_array : bool_constant<is_unbounded_array_v<T>> {};

	template <class T> inline constexpr bool is_lvalue_reference_v = false;
	template <class T> inline constexpr bool is_lvalue_reference_v<T&> = true;
	template <class T> struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<T>> {};

	template <class T> inline constexpr bool is_rvalue_reference_v = false;
	template <class T> inline constexpr bool is_rvalue_reference_v<T&&> = true;
	template <class T> struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<T>> {};

	template <class T> inline constexpr bool is_reference_v = false;
	template <class T> inline constexpr bool is_reference_v<T&> = true;
	template <class T> inline constexpr bool is_reference_v<T&&> = true;
	template <class T> struct is_reference : bool_constant<is_reference_v<T>> {};

	template <class T> inline constexpr bool is_pointer_v = false;
	template <class T> inline constexpr bool is_pointer_v<T*> = true;
	template <class T> inline constexpr bool is_pointer_v<T* const> = true;
	template <class T> inline constexpr bool is_pointer_v<T* volatile> = true;
	template <class T> inline constexpr bool is_pointer_v<T* const volatile> = true;
	template <class T> struct is_pointer : bool_constant<is_pointer_v<T>> {};

	template <class T> inline constexpr bool is_null_pointer_v = is_same_v<remove_cv_t<T>, nullptr_t>;
	template <class T> struct is_null_pointer : bool_constant<is_null_pointer_v<T>> {};

	template <class T> inline constexpr bool is_union_v = __is_union(T);
	template <class T> struct is_union : bool_constant<__is_union(T)> {};

	template <class T> inline constexpr bool is_class_v = __is_class(T);
	template <class T> struct is_class : bool_constant<__is_class(T)> {};

	template <class T> inline constexpr bool is_fundamental_v = is_arithmetic_v<T> || is_void_v<T> || is_null_pointer_v<T>;
	template <class T> struct is_fundamental : bool_constant<is_fundamental_v<T>> {};

	template <class From, class To> inline constexpr bool is_convertible_v = __is_convertible_to(From, To);
	template <class T> inline constexpr bool is_convertible_v<T&, volatile T&> = true;
	template <class T> inline constexpr bool is_convertible_v<volatile T&, volatile T&> = true;
	template <class T> inline constexpr bool is_convertible_v<T&, const volatile T&> = true;
	template <class T> inline constexpr bool is_convertible_v<volatile T&, const volatile T&> = true;
	template <class From, class To> struct is_convertible : bool_constant<__is_convertible_to(From, To)> {};
	template <class T> struct is_convertible<T&, volatile T&> : true_type {};
	template <class T> struct is_convertible<volatile T&, volatile T&> : true_type {};
	template <class T> struct is_convertible<T&, const volatile T&> : true_type {};
	template <class T> struct is_convertible<volatile T&, const volatile T&> : true_type {};

	template <class T> inline constexpr bool is_enum_v = __is_enum(T);
	template <class T> struct is_enum : bool_constant<__is_enum(T)> {};

	template <class T> inline constexpr bool is_scoped_enum_v = conjunction_v<is_enum<T>, negation<is_convertible<T, int>>>;
	template <class T> struct is_scoped_enum : bool_constant<is_scoped_enum_v<T>> {};

	template <class T> inline constexpr bool is_compound_v = !is_fundamental_v<T>;
	template <class T> struct is_compound : bool_constant<!is_fundamental_v<T>> {};

	template <class T> struct _Is_memfunptr { using _Bool_type = false_type; };
	template <class T> inline constexpr bool is_member_function_pointer_v = _Is_memfunptr<remove_cv_t<T>>::_Bool_type::value;
	template <class T> struct is_member_function_pointer : bool_constant<is_member_function_pointer_v<T>> {};

	template <class T> inline constexpr bool is_const_v = false;
	template <class T> inline constexpr bool is_const_v<const T> = true;
	template <class T> struct is_const : bool_constant<is_const_v<T>> {};

	template <class> inline constexpr bool is_volatile_v = false;
	template <class T> inline constexpr bool is_volatile_v<volatile T> = true;
	template <class T> struct is_volatile : bool_constant<is_volatile_v<T>> {};

	template <class T> inline constexpr bool is_function_v = !is_const_v<const T> && !is_reference_v<T>;
	template <class T> struct is_function : bool_constant<is_function_v<T>> {};

	template <class T> inline constexpr bool is_object_v = is_const_v<const T> && !is_void_v<T>;
	template <class T> struct is_object : bool_constant<is_object_v<T>> {};

	template <class> struct _Is_member_object_pointer { static constexpr bool value = false; };

	template <class T1, class T2>
	struct _Is_member_object_pointer<T1 T2::*> {
		static constexpr bool value = !is_function_v<T1>;
		using _Class_type = T2;
	};

	template <class T> inline constexpr bool is_member_object_pointer_v = _Is_member_object_pointer<remove_cv_t<T>>::value;
	template <class T> struct is_member_object_pointer : bool_constant<is_member_object_pointer_v<T>> {};

	template <class T> inline constexpr bool is_member_pointer_v = is_member_object_pointer_v<T> || is_member_function_pointer_v<T>;
	template <class T> struct is_member_pointer : bool_constant<is_member_pointer_v<T>> {};

	template <class T> inline constexpr bool is_scalar_v = is_arithmetic_v<T> || is_enum_v<T> || is_pointer_v<T> || is_member_pointer_v<T> || is_null_pointer_v<T>;
	template <class T> struct is_scalar : bool_constant<is_scalar_v<T>> {};

	template <class T> inline constexpr bool is_empty_v = __is_empty(T);
	template <class T> struct is_empty : bool_constant<__is_empty(T)> {};

	template <class T> inline constexpr bool is_polymorphic_v = __is_polymorphic(T);
	template <class T> struct is_polymorphic : bool_constant<__is_polymorphic(T)> {};

	template <class T> inline constexpr bool is_abstract_v = __is_abstract(T);
	template <class T> struct is_abstract : bool_constant<__is_abstract(T)> {};

	template <class T> inline constexpr bool is_final_v = __is_final(T);
	template <class T> struct is_final : bool_constant<__is_final(T)> {};

	template <class T> inline constexpr bool is_standard_layout_v = __is_standard_layout(T);
	template <class T> struct is_standard_layout : bool_constant<__is_standard_layout(T)> {};

	template <class T> inline constexpr bool is_trivial_v = __is_trivially_constructible(T) && __is_trivially_copyable(T);
	template <class T> struct is_trivial : bool_constant<__is_trivially_constructible(T) && __is_trivially_copyable(T)> {};

	template <class T> inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T);
	template <class T> struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

	template <class T> inline constexpr bool has_virtual_destructor_v = __has_virtual_destructor(T);
	template <class T> struct has_virtual_destructor : bool_constant<__has_virtual_destructor(T)> {};

	template <class T> inline constexpr bool has_unique_object_representations_v = __has_unique_object_representations(T);
	template <class T> struct has_unique_object_representations : bool_constant<__has_unique_object_representations(T)> {};

	template <class T> inline constexpr bool is_aggregate_v = __is_aggregate(T);
	template <class T> struct is_aggregate : bool_constant<__is_aggregate(T)> {};

	template <class T, class... Args> inline constexpr bool is_constructible_v = __is_constructible(T, Args...);
	template <class T, class... Args> struct is_constructible : bool_constant<__is_constructible(T, Args...)> {	};

	template <class T> inline constexpr bool is_copy_constructible_v = __is_constructible(T, add_lvalue_reference_t<const T>);
	template <class T> struct is_copy_constructible : bool_constant<__is_constructible(T, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_default_constructible_v = __is_constructible(T);
	template <class T> struct is_default_constructible : bool_constant<__is_constructible(T)> {};

	template <class T, class = void> struct _Is_implicitly_default_constructible : false_type {};
	template <class T> void _Implicitly_default_construct(const T&);
	template <class T> struct _Is_implicitly_default_constructible < T, void_t<decltype(_Implicitly_default_construct<T>({})) >> : true_type {};

	template <class T> inline constexpr bool is_move_constructible_v = __is_constructible(T, T);
	template <class T> struct is_move_constructible : bool_constant<__is_constructible(T, T)> {};

	template <class To, class From> inline constexpr bool is_assignable_v = __is_assignable(To, From);
	template <class To, class From> struct is_assignable : bool_constant<__is_assignable(To, From)> {};

	template <class _To, class _From> struct _Is_assignable_no_precondition_check : bool_constant<__is_assignable_no_precondition_check(_To, _From)> {};

	template <class T> inline constexpr bool _Is_copy_assignable_unchecked_v = __is_assignable_no_precondition_check(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>);
	template <class T> struct _Is_copy_assignable_no_precondition_check : bool_constant<__is_assignable_no_precondition_check( add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_copy_assignable_v = __is_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>);
	template <class T> struct is_copy_assignable : bool_constant<__is_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_move_assignable_v = __is_assignable(add_lvalue_reference_t<T>, T);
	template <class T> struct is_move_assignable : bool_constant<__is_assignable(add_lvalue_reference_t<T>, T)> {};

	template <class T> inline constexpr bool _Is_move_assignable_unchecked_v = __is_assignable_no_precondition_check(add_lvalue_reference_t<T>, T);
	template <class T> struct _Is_move_assignable_no_precondition_check : bool_constant<__is_assignable_no_precondition_check(add_lvalue_reference_t<T>, T)> {};

	template <class T> inline constexpr bool is_destructible_v = __is_destructible(T);
	template <class T> struct is_destructible : bool_constant<__is_destructible(T)> {};

	template <class T, class... _Args> inline constexpr bool is_trivially_constructible_v = __is_trivially_constructible(T, _Args...);
	template <class T, class... _Args> struct is_trivially_constructible : bool_constant<__is_trivially_constructible(T, _Args...)> {};

	template <class T> inline constexpr bool is_trivially_copy_constructible_v = __is_trivially_constructible(T, add_lvalue_reference_t<const T>);
	template <class T> struct is_trivially_copy_constructible : bool_constant<__is_trivially_constructible(T, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_trivially_default_constructible_v = __is_trivially_constructible(T);
	template <class T> struct is_trivially_default_constructible : bool_constant<__is_trivially_constructible(T)> {};

	template <class T> inline constexpr bool is_trivially_move_constructible_v = __is_trivially_constructible(T, T);
	template <class T> struct is_trivially_move_constructible : bool_constant<__is_trivially_constructible(T, T)> {};

	template <class To, class From> inline constexpr bool is_trivially_assignable_v = __is_trivially_assignable(To, From);
	template <class To, class From> struct is_trivially_assignable : bool_constant<__is_trivially_assignable(To, From)> {};

	template <class T> inline constexpr bool is_trivially_copy_assignable_v = __is_trivially_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>);
	template <class T> struct is_trivially_copy_assignable : bool_constant<__is_trivially_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_trivially_move_assignable_v = __is_trivially_assignable(add_lvalue_reference_t<T>, T);
	template <class T> struct is_trivially_move_assignable : bool_constant<__is_trivially_assignable(add_lvalue_reference_t<T>, T)> {};

	template <class T> inline constexpr bool is_trivially_destructible_v = __is_trivially_destructible(T);
	template <class T> struct is_trivially_destructible : bool_constant<__is_trivially_destructible(T)> {};

	template <class T, class... _Args> inline constexpr bool is_nothrow_constructible_v = __is_nothrow_constructible(T, _Args...);
	template <class T, class... _Args> struct is_nothrow_constructible : bool_constant<__is_nothrow_constructible(T, _Args...)> {};

	template <class T> inline constexpr bool is_nothrow_copy_constructible_v = __is_nothrow_constructible(T, add_lvalue_reference_t<const T>);
	template <class T> struct is_nothrow_copy_constructible : bool_constant<__is_nothrow_constructible(T, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_nothrow_default_constructible_v = __is_nothrow_constructible(T);
	template <class T> struct is_nothrow_default_constructible : bool_constant<__is_nothrow_constructible(T)> {};

	template <class T> inline constexpr bool is_nothrow_move_constructible_v = __is_nothrow_constructible(T, T);
	template <class T> struct is_nothrow_move_constructible : bool_constant<__is_nothrow_constructible(T, T)> {};

	template <class To, class From> inline constexpr bool is_nothrow_assignable_v = __is_nothrow_assignable(To, From);
	template <class To, class From> struct is_nothrow_assignable : bool_constant<__is_nothrow_assignable(To, From)> {};

	template <class T> inline constexpr bool is_nothrow_copy_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>);
	template <class T> struct is_nothrow_copy_assignable : bool_constant<__is_nothrow_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_nothrow_move_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<T>, T);
	template <class T> struct is_nothrow_move_assignable : bool_constant<__is_nothrow_assignable(add_lvalue_reference_t<T>, T)> {};

	template <class T> inline constexpr bool is_nothrow_destructible_v = __is_nothrow_destructible(T);
	template <class T> struct is_nothrow_destructible : bool_constant<__is_nothrow_destructible(T)> {};

	template <class T> struct _Is_swappable;
	template <class T> struct _Is_nothrow_swappable;


	template <class T, enable_if_t<is_move_constructible_v<T>&& is_move_assignable_v<T>, int> = 0> constexpr void swap(T&, T&) noexcept(is_nothrow_move_constructible_v<T>&& is_nothrow_move_assignable_v<T>);
	template <class T, size_t _Size, enable_if_t<_Is_swappable<T>::value, int> = 0> constexpr void swap(T(&)[_Size], T(&)[_Size]) noexcept(_Is_nothrow_swappable<T>::value);

	template <class T1, class T2, class = void> struct _Swappable_with_helper : false_type {};

	template <class T1, class T2> struct _Swappable_with_helper<T1, T2, void_t<decltype(swap(declval<T1>(), declval<T2>()))>> : true_type {};

	template <class T1, class T2> struct _Is_swappable_with : bool_constant<conjunction_v<_Swappable_with_helper<T1, T2>, _Swappable_with_helper<T2, T1>>> {};

	template <class T> struct _Is_swappable : _Is_swappable_with<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type {};

	template <class T1, class T2> struct _Swap_cannot_throw : bool_constant<noexcept(swap(declval<T1>(), declval<T2>())) && noexcept(swap(declval<T2>(), declval<T1>()))> {};

	template <class T1, class T2> struct _Is_nothrow_swappable_with : bool_constant<conjunction_v<_Is_swappable_with<T1, T2>, _Swap_cannot_throw<T1, T2>>> {};

	template <class T> struct _Is_nothrow_swappable : _Is_nothrow_swappable_with<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type {};

	template <class T1, class T2> inline constexpr bool is_swappable_with_v = conjunction_v<_Swappable_with_helper<T1, T2>, _Swappable_with_helper<T2, T1>>;
	template <class T1, class T2> struct is_swappable_with : _Is_swappable_with<T1, T2>::type {};

	template <class T> inline constexpr bool is_swappable_v = _Is_swappable<T>::value;
	template <class T> struct is_swappable : _Is_swappable<T>::type {};

	template <class T1, class T2> inline constexpr bool is_nothrow_swappable_with_v = _Is_nothrow_swappable_with<T1, T2>::value;
	template <class T1, class T2> struct is_nothrow_swappable_with : _Is_nothrow_swappable_with<T1, T2>::type {};

	template <class T> inline constexpr bool is_nothrow_swappable_v = _Is_nothrow_swappable<T>::value;
	template <class T> struct is_nothrow_swappable : _Is_nothrow_swappable<T>::type {};



	template <class T, bool = is_integral_v<T>>
	struct _Sign_base
	{
		using _Uty = remove_cv_t<T>;

		static constexpr bool _Signed = static_cast<_Uty>(-1) < static_cast<_Uty>(0);
		static constexpr bool _Unsigned = !_Signed;
	};

	template <class T>
	struct _Sign_base<T, false>
	{
		static constexpr bool _Signed = is_floating_point_v<T>;
		static constexpr bool _Unsigned = false;
	};

	template <class T> inline constexpr bool is_signed_v = _Sign_base<T>::_Signed;
	template <class T> struct is_signed : bool_constant<_Sign_base<T>::_Signed> {};

	template <class T> inline constexpr bool is_unsigned_v = _Sign_base<T>::_Unsigned;
	template <class T> struct is_unsigned : bool_constant<_Sign_base<T>::_Unsigned> {};

	template <class T> inline constexpr bool _Is_nonbool_integral = is_integral_v<T> && !is_same_v<remove_cv_t<T>, bool>;

	template <bool>
	struct _Select
	{
		template <class T1, class> using _Apply = T1;
	};

	template <>
	struct _Select<false>
	{
		template <class, class T2> using _Apply = T2;
	};

	template <size_t>
	struct _Make_signed2;

	template <>
	struct _Make_signed2<1> {
		template <class> using _Apply = signed char;
	};

	template <>
	struct _Make_signed2<2> {
		template <class> using _Apply = short;
	};

	template <>
	struct _Make_signed2<4> {
		template <class T> using _Apply = typename _Select<is_same_v<T, long> || is_same_v<T, unsigned long>>::template _Apply<long, int>;
	};

	template <>
	struct _Make_signed2<8> {
		template <class> using _Apply = long long;
	};

	template <class T>
	using _Make_signed1 = typename _Make_signed2<sizeof(T)>::template _Apply<T>;

	template <class T>
	struct make_signed
	{
		static_assert(_Is_nonbool_integral<T> || is_enum_v<T>,
			"make_signed<T> requires that T shall be a (possibly cv-qualified) "
			"integral type or enumeration but not a bool type.");

		using type = typename remove_cv<T>::template _Apply<_Make_signed1>;
	};

	template <class T>
	using make_signed_t = typename make_signed<T>::type;

	template <size_t>
	struct _Make_unsigned2;

	template <>
	struct _Make_unsigned2<1>
	{
		template <class> using _Apply = unsigned char;
	};

	template <>
	struct _Make_unsigned2<2>
	{
		template <class> using _Apply = unsigned short;
	};

	template <>
	struct _Make_unsigned2<4>
	{
		template <class T> using _Apply = typename _Select<is_same_v<T, long> || is_same_v<T, unsigned long>>::template _Apply<unsigned long, unsigned int>;
	};

	template <>
	struct _Make_unsigned2<8>
	{
		template <class> using _Apply = unsigned long long;
	};

	template <class T>
	using _Make_unsigned1 = typename _Make_unsigned2<sizeof(T)>::template _Apply<T>;

	template <class T>
	struct make_unsigned
	{
		static_assert(_Is_nonbool_integral<T> || is_enum_v<T>,
			"make_unsigned<T> requires that T shall be a (possibly cv-qualified) "
			"integral type or enumeration but not a bool type.");

		using type = typename remove_cv<T>::template _Apply<_Make_unsigned1>;
	};

	template <class T>
	using make_unsigned_t = typename make_unsigned<T>::type;

	template <class _Rep>
	constexpr make_unsigned_t<_Rep> _Unsigned_value(_Rep _Val)
	{
		return static_cast<make_unsigned_t<_Rep>>(_Val);
	}

	template <class T, bool = is_enum_v<T>>
	struct _Underlying_type
	{
		using type = __underlying_type(T);
	};
	template <class T> struct _Underlying_type<T, false> {};
	template <class T>struct underlying_type : _Underlying_type<T> {};
	template <class T> using underlying_type_t = typename _Underlying_type<T>::type;

	template <class T> inline constexpr size_t alignment_of_v = alignof(T);
	template <class T>struct alignment_of : integral_constant<size_t, alignof(T)> {};

	template <class Base, class Derived> inline constexpr bool is_base_of_v = __is_base_of(Base, Derived);
	template <class Base, class Derived> struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};

	template <class T>
	struct decay {
		using T1 = remove_reference_t<T>;
		using T2 = typename _Select<is_function_v<T1>>::template _Apply<add_pointer<T1>, remove_cv<T1>>;
		using type = typename _Select<is_array_v<T1>>::template _Apply<add_pointer<remove_extent_t<T1>>, T2>::type;
	};

	template <class T>
	using decay_t = typename decay<T>::type;

	template <class... T>
	struct common_type;

	template <class... T>
	using common_type_t = typename common_type<T...>::type;

	template <>
	struct common_type<> {};

	template <class T>
	struct common_type<T> : common_type<T, T> {};

	template <class...>
	struct common_reference;

	template <class... T>
	using common_reference_t = typename common_reference<T...>::type;

	template <>
	struct common_reference<> {};

	template <class T>
	struct common_reference<T> {
		using type = T;
	};

	template <class To>
	void _Implicitly_convert_to(To) noexcept;

	template <class From, class To, bool = is_convertible_v<From, To>, bool = is_void_v<To>>
	inline constexpr bool _Is_nothrow_convertible_v = noexcept(_Implicitly_convert_to<To>(declval<From>()));

	template <class From, class To, bool IsVoid>
	inline constexpr bool _Is_nothrow_convertible_v<From, To, false, IsVoid> = false;

	template <class From, class To>
	inline constexpr bool _Is_nothrow_convertible_v<From, To, true, true> = true;

	template <class From, class To>
	struct _Is_nothrow_convertible : bool_constant<_Is_nothrow_convertible_v<From, To>> {};

	template <class From, class To>
	inline constexpr bool is_nothrow_convertible_v = _Is_nothrow_convertible_v<From, To>;

	template <class _Iter, class _Pointer, bool = is_pointer_v<_Remove_cvref_t<_Iter>>>
	inline constexpr bool _Has_nothrow_operator_arrow = _Is_nothrow_convertible_v<_Iter, _Pointer>;

	template <class _Iter, class _Pointer>
	inline constexpr bool _Has_nothrow_operator_arrow<_Iter, _Pointer, false> = noexcept(_Fake_copy_init<_Pointer>(declval<_Iter>().operator->()));

	template <class From, class To>
	using is_nothrow_convertible = _Is_nothrow_convertible<From, To>;

	template <class Type, template <class...> class Template>
	inline constexpr bool _Is_specialization_v = false;
	template <template <class...> class Template, class... Types>
	inline constexpr bool _Is_specialization_v<Template<Types...>, Template> = true;

	template <class T>
	[[nodiscard]] constexpr T&& forward(remove_reference_t<T>& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}

	template <class T>
	[[nodiscard]] constexpr T&& forward(remove_reference_t<T>&& arg) noexcept
	{
		static_assert(!is_lvalue_reference_v<T>, "bad forward call");
		return static_cast<T&&>(arg);
	}

	template <class T>
	[[nodiscard]] constexpr remove_reference_t<T>&& move(T&& arg) noexcept
	{
		return static_cast<remove_reference_t<T>&&>(arg);
	}

	template <class T>
	[[nodiscard]] constexpr conditional_t<!is_nothrow_move_constructible_v<T>&& is_copy_constructible_v<T>, const T&, T&&> move_if_noexcept(T& arg) noexcept
	{
		return move(arg);
	}

	template <class T>
	class reference_wrapper;

	// std::invoke isn't constexpr in C++17, and normally implementers are forbidden from "strengthening" constexpr
	// (WG21-N4842 [constexpr.functions]/1), yet both std::apply and std::visit are required to be constexpr and have
	// invoke-like behavior. As a result, we've chosen to apply the part of P1065R2 resolving LWG-2894 as a defect report.

	enum class _Invoker_strategy {
		_Functor,
		_Pmf_object,
		_Pmf_refwrap,
		_Pmf_pointer,
		_Pmd_object,
		_Pmd_refwrap,
		_Pmd_pointer
	};

	struct _Invoker_functor {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Functor;

		template <class _Callable, class... _Types>
		static constexpr auto _Call(_Callable&& _Obj, _Types&&... _Args) noexcept(
			noexcept(static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...)))
			-> decltype(static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...)) {
			return static_cast<_Callable&&>(_Obj)(static_cast<_Types&&>(_Args)...);
		}
	};

	struct _Invoker_pmf_object {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_object;

		template <class _Decayed, class T1, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, T1&& _Arg1, _Types2&&... _Args2) noexcept(
			noexcept((static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype((static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return (static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmf_refwrap {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_refwrap;

		template <class _Decayed, class _Refwrap, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, _Refwrap _Rw, _Types2&&... _Args2) noexcept(
			noexcept((_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype((_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return (_Rw.get().*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmf_pointer {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_pointer;

		template <class _Decayed, class T1, class... _Types2>
		static constexpr auto _Call(_Decayed _Pmf, T1&& _Arg1, _Types2&&... _Args2) noexcept(
			noexcept(((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...)))
			-> decltype(((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...)) {
			return ((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<_Types2&&>(_Args2)...);
		}
	};

	struct _Invoker_pmd_object {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_object;

		template <class _Decayed, class T1>
		static constexpr auto _Call(_Decayed _Pmd, T1&& _Arg1) noexcept -> decltype(static_cast<T1&&>(_Arg1).*_Pmd) {
			return static_cast<T1&&>(_Arg1).*_Pmd;
		}
	};

	struct _Invoker_pmd_refwrap {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_refwrap;

		template <class _Decayed, class _Refwrap>
		static constexpr auto _Call(_Decayed _Pmd, _Refwrap _Rw) noexcept -> decltype(_Rw.get().*_Pmd) {
			return _Rw.get().*_Pmd;
		}
	};

	struct _Invoker_pmd_pointer {
		static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_pointer;

		template <class _Decayed, class T1>
		static constexpr auto _Call(_Decayed _Pmd, T1&& _Arg1) noexcept(noexcept((*static_cast<T1&&>(_Arg1)).*_Pmd))
			-> decltype((*static_cast<T1&&>(_Arg1)).*_Pmd) {
			return (*static_cast<T1&&>(_Arg1)).*_Pmd;
		}
	};

	template <class _Callable, class T1, class _Removed_cvref = _Remove_cvref_t<_Callable>,
		bool _Is_pmf = is_member_function_pointer_v<_Removed_cvref>,
		bool _Is_pmd = is_member_object_pointer_v<_Removed_cvref>>
		struct _Invoker1;

	template <class _Callable, class T1, class _Removed_cvref>
	struct _Invoker1<_Callable, T1, _Removed_cvref, true, false>
		: conditional_t<is_base_of_v<typename _Is_memfunptr<_Removed_cvref>::_Class_type, remove_reference_t<T1>>,
		_Invoker_pmf_object,
		conditional_t<_Is_specialization_v<_Remove_cvref_t<T1>, reference_wrapper>, _Invoker_pmf_refwrap,
		_Invoker_pmf_pointer>> {}; // pointer to member function

	template <class _Callable, class T1, class _Removed_cvref>
	struct _Invoker1<_Callable, T1, _Removed_cvref, false, true>
		: conditional_t<
		is_base_of_v<typename _Is_member_object_pointer<_Removed_cvref>::_Class_type, remove_reference_t<T1>>,
		_Invoker_pmd_object,
		conditional_t<_Is_specialization_v<_Remove_cvref_t<T1>, reference_wrapper>, _Invoker_pmd_refwrap,
		_Invoker_pmd_pointer>> {}; // pointer to member data

	template <class _Callable, class T1, class _Removed_cvref>
	struct _Invoker1<_Callable, T1, _Removed_cvref, false, false> : _Invoker_functor {};

	template <class T>
	T _Returns_exactly() noexcept; // not defined

	template <class From, class To, class = void>
	struct _Invoke_convertible : false_type {};

	template <class From, class To>
	struct _Invoke_convertible<From, To, void_t<decltype(_Implicitly_convert_to<To>(_Returns_exactly<From>()))>>
		: true_type {};

	template <class From, class To>
	struct _Invoke_nothrow_convertible : bool_constant<noexcept(_Implicitly_convert_to<To>(_Returns_exactly<From>()))> {};

	template <class _Result, bool _Nothrow>
	struct _Invoke_traits_common {
		using type = _Result;
		using _Is_invocable = true_type;
		using _Is_nothrow_invocable = bool_constant<_Nothrow>;
		template <class _Rx>
		using _Is_invocable_r = bool_constant<disjunction_v<is_void<_Rx>, _Invoke_convertible<type, _Rx>>>;
		template <class _Rx>
		using _Is_nothrow_invocable_r = bool_constant<conjunction_v<_Is_nothrow_invocable,
			disjunction<is_void<_Rx>,
			conjunction<_Invoke_convertible<type, _Rx>, _Invoke_nothrow_convertible<type, _Rx>>>>>;
	};

	template <class _Void, class _Callable>
	struct _Invoke_traits_zero {
		// selected when _Callable isn't callable with zero _Args
		using _Is_invocable = false_type;
		using _Is_nothrow_invocable = false_type;
		template <class _Rx>
		using _Is_invocable_r = false_type;
		template <class _Rx>
		using _Is_nothrow_invocable_r = false_type;
	};

	template <class _Callable>
	using _Decltype_invoke_zero = decltype(declval<_Callable>()());

	template <class _Callable>
	struct _Invoke_traits_zero<void_t<_Decltype_invoke_zero<_Callable>>, _Callable>
		: _Invoke_traits_common<_Decltype_invoke_zero<_Callable>, noexcept(declval<_Callable>()())> {};

	template <class _Void, class... _Types>
	struct _Invoke_traits_nonzero {
		using _Is_invocable = false_type;
		using _Is_nothrow_invocable = false_type;
		template <class _Rx>
		using _Is_invocable_r = false_type;
		template <class _Rx>
		using _Is_nothrow_invocable_r = false_type;
	};

	template <class _Callable, class T1, class... _Types2>
	using _Decltype_invoke_nonzero = decltype(_Invoker1<_Callable, T1>::_Call(
		declval<_Callable>(), declval<T1>(), declval<_Types2>()...));


	template <class _Callable, class T1, class... _Types2>
	struct _Invoke_traits_nonzero<void_t<_Decltype_invoke_nonzero<_Callable, T1, _Types2...>>, _Callable, T1,
		_Types2...> : _Invoke_traits_common<_Decltype_invoke_nonzero<_Callable, T1, _Types2...>,
		noexcept(_Invoker1<_Callable, T1>::_Call(
			declval<_Callable>(), declval<T1>(), declval<_Types2>()...))> {};

	template <class _Callable, class... _Args>
	using _Select_invoke_traits = conditional_t<sizeof...(_Args) == 0, _Invoke_traits_zero<void, _Callable>,
		_Invoke_traits_nonzero<void, _Callable, _Args...>>;

	template <class _Callable, class... _Args>
	using _Invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

	template <class _Rx, class _Callable, class... _Args>
	using _Is_invocable_r_ = typename _Select_invoke_traits<_Callable, _Args...>::template _Is_invocable_r<_Rx>;

	template <class _Rx, class _Callable, class... _Args>
	struct _Is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {};

	template <class _Callable, class... _Args>
	struct invoke_result : _Select_invoke_traits<_Callable, _Args...> {};

	template <class _Callable, class... _Args>
	using invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

	template <class _Callable, class... _Args>
	struct is_invocable : _Select_invoke_traits<_Callable, _Args...>::_Is_invocable {};

	template <class _Callable, class... _Args>
	inline constexpr bool is_invocable_v = _Select_invoke_traits<_Callable, _Args...>::_Is_invocable::value;

	template <class _Callable, class... _Args>
	struct is_nothrow_invocable : _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable {};

	template <class _Callable, class... _Args>
	inline constexpr bool is_nothrow_invocable_v = _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable::value;

	template <class _Rx, class _Callable, class... _Args>
	struct is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {};

	template <class _Rx, class _Callable, class... _Args>
	inline constexpr bool is_invocable_r_v = _Is_invocable_r_<_Rx, _Callable, _Args...>::value;

	template <class _Rx, class _Callable, class... _Args>
	struct is_nothrow_invocable_r : _Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx> {};

	template <class _Rx, class _Callable, class... _Args>
	inline constexpr bool is_nothrow_invocable_r_v =
		_Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx>::value;

	template <class _FloatingType>
	struct _Floating_type_traits;

	template <>
	struct _Floating_type_traits<float> {
		static constexpr int _Mantissa_bits = 24; // FLT_MANT_DIG
		static constexpr int _Exponent_bits = 8; // sizeof(float) * CHAR_BIT - FLT_MANT_DIG
		static constexpr int _Maximum_binary_exponent = 127; // FLT_MAX_EXP - 1
		static constexpr int _Minimum_binary_exponent = -126; // FLT_MIN_EXP - 1
		static constexpr int _Exponent_bias = 127;
		static constexpr int _Sign_shift = 31; // _Exponent_bits + _Mantissa_bits - 1
		static constexpr int _Exponent_shift = 23; // _Mantissa_bits - 1

		using _Uint_type = unsigned int;

		static constexpr unsigned int _Exponent_mask = 0x000000FFu; // (1u << _Exponent_bits) - 1
		static constexpr unsigned int _Normal_mantissa_mask = 0x00FFFFFFu; // (1u << _Mantissa_bits) - 1
		static constexpr unsigned int _Denormal_mantissa_mask = 0x007FFFFFu; // (1u << (_Mantissa_bits - 1)) - 1
		static constexpr unsigned int _Special_nan_mantissa_mask = 0x00400000u; // 1u << (_Mantissa_bits - 2)
		static constexpr unsigned int _Shifted_sign_mask = 0x80000000u; // 1u << _Sign_shift
		static constexpr unsigned int _Shifted_exponent_mask = 0x7F800000u; // _Exponent_mask << _Exponent_shift
	};

	template <>
	struct _Floating_type_traits<double> {
		static constexpr int _Mantissa_bits = 53; // DBL_MANT_DIG
		static constexpr int _Exponent_bits = 11; // sizeof(double) * CHAR_BIT - DBL_MANT_DIG
		static constexpr int _Maximum_binary_exponent = 1023; // DBL_MAX_EXP - 1
		static constexpr int _Minimum_binary_exponent = -1022; // DBL_MIN_EXP - 1
		static constexpr int _Exponent_bias = 1023;
		static constexpr int _Sign_shift = 63; // _Exponent_bits + _Mantissa_bits - 1
		static constexpr int _Exponent_shift = 52; // _Mantissa_bits - 1

		using _Uint_type = unsigned long long;

		static constexpr unsigned long long _Exponent_mask = 0x00000000000007FFu; // (1ULL << _Exponent_bits) - 1
		static constexpr unsigned long long _Normal_mantissa_mask = 0x001FFFFFFFFFFFFFu; // (1ULL << _Mantissa_bits) - 1
		static constexpr unsigned long long _Denormal_mantissa_mask = 0x000FFFFFFFFFFFFFu; // (1ULL << (_Mantissa_bits - 1)) - 1
		static constexpr unsigned long long _Special_nan_mantissa_mask = 0x0008000000000000u; // 1ULL << (_Mantissa_bits - 2)
		static constexpr unsigned long long _Shifted_sign_mask = 0x8000000000000000u; // 1ULL << _Sign_shift
		static constexpr unsigned long long _Shifted_exponent_mask = 0x7FF0000000000000u; // _Exponent_mask << _Exponent_shift
	};
}