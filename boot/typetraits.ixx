export module typetraits;

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

	template <class Trait> inline constexpr bool negation_v = negation<Trait>::value;
	template <class Trait> struct negation : bool_constant<!static_cast<bool>(Trait::value)> {};

	template <class T> inline constexpr bool is_void_v = is_same_v<remove_cv_t<T>, void>;
	template <class T> struct is_void : bool_constant<is_void_v<T>> {};

	template <class... Types> using void_t = void;

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

	template <class T> struct add_lvalue_reference { using type = typename Add_reference<T>::_Lvalue; };
	template <class T> using add_lvalue_reference_t = typename Add_reference<T>::_Lvalue;
	template <class T> struct add_rvalue_reference { using type = typename Add_reference<T>::_Rvalue; };
	template <class T> using add_rvalue_reference_t = typename Add_reference<T>::_Rvalue;
	template <class T> add_rvalue_reference_t<T> declval() noexcept { static_assert(Always_false<T>, "Calling declval is ill-formed, see N4892 [declval]/2."); }

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

	template <class T> inline constexpr bool is_member_function_pointer_v = _Is_memfunptr<remove_cv_t<T>>::_Bool_type::value;
	template <class T> struct _Is_memfunptr { using _Bool_type = false_type; };
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


	template <class T> inline constexpr bool is_copy_assignable_v = __is_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>);
	template <class T> struct is_copy_assignable : bool_constant<__is_assignable(add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>)> {};

	template <class T> inline constexpr bool is_move_assignable_v = __is_assignable(add_lvalue_reference_t<T>, T);
	template <class T> struct is_move_assignable : bool_constant<__is_assignable(add_lvalue_reference_t<T>, T)> {};

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

	template <class T> inline constexpr size_t alignment_of_v = alignof(T);
	template <class T>struct alignment_of : integral_constant<size_t, alignof(T)> {};

	template <class Base, class Derived> inline constexpr bool is_base_of_v = __is_base_of(Base, Derived);
	template <class Base, class Derived> struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};

}