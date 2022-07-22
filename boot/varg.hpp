#pragma once

using varg = char*;
extern "C++"
{
    template <typename _Ty>
    struct __vcrt_va_list_is_reference
    {
        enum : bool { __the_value = false };
    };
    template <typename _Ty>
    struct __vcrt_va_list_is_reference<_Ty&>
    {
        enum : bool { __the_value = true };
    };
    template <typename _Ty>
    struct __vcrt_va_list_is_reference<_Ty&&>
    {
        enum : bool { __the_value = true };
    };
    template <typename _Ty>
    struct __vcrt_assert_va_start_is_not_reference
    {
        static_assert(!__vcrt_va_list_is_reference<_Ty>::__the_value,
            "va_start argument must not have reference type and must not be parenthesized");
    };

} // extern "C++"
extern "C" void __cdecl __va_start(varg*, ...);
template <typename T>
constexpr void varg_start(varg ap, T x, ...)
{
    __vcrt_assert_va_start_is_not_reference<decltype(x)>();
	__va_start(&ap, x);
}
template <typename T>
constexpr T varg_arg(varg ap)
{
   return (sizeof(T) > sizeof(__int64) || (sizeof(T) & (sizeof(T) - 1)) != 0) ? **(T**)((ap += sizeof(__int64)) - sizeof(__int64)) : *(T*)((ap += sizeof(__int64)) - sizeof(__int64));
}
constexpr void varg_end(varg ap)
{
    ap = 0;
}
