#pragma once
using va_list = char*;
extern "C"
{
	void __cdecl __va_start(va_list* , ...);

	#define __crt_va_start_a(ap, x) ((void)(__va_start(&ap, x)))
	#define __crt_va_arg(ap, t)                                               \
		((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
			? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))             \
			:  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
	#define __crt_va_end(ap)        ((void)(ap = (va_list)0))
}

extern "C++"
{
	template <typename T>
	struct __vcrt_va_list_is_reference
	{
		enum : bool { __the_value = false };
	};

	template <typename T>
	struct __vcrt_va_list_is_reference<T&>
	{
		enum : bool { __the_value = true };
	};

	template <typename T>
	struct __vcrt_va_list_is_reference<T&&>
	{
		enum : bool { __the_value = true };
	};

	template <typename T>
	struct __vcrt_assert_va_start_is_not_reference
	{
		static_assert(!__vcrt_va_list_is_reference<T>::__the_value,
			"va_start argument must not have reference type and must not be parenthesized");
	};
}
#define __crt_va_start(ap, x) ((void)(__vcrt_assert_va_start_is_not_reference<decltype(x)>(), __crt_va_start_a(ap, x)))