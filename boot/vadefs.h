//
// vadefs.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Definitions of macro helpers used by <stdarg.h>.  This is the topmost header
// in the CRT header lattice, and is always the first CRT header to be included,
// explicitly or implicitly.  Therefore, this header also has several definitions
// that are used throughout the CRT.
//
#pragma once
#define _INC_VADEFS

#define _CRT_PACKING 8
#pragma pack(push, _CRT_PACKING)

// Use _VCRUNTIME_EXTRA_DISABLED_WARNINGS to add additional warning suppressions to VCRuntime headers.
#ifndef _VCRUNTIME_EXTRA_DISABLED_WARNINGS
    #define _VCRUNTIME_EXTRA_DISABLED_WARNINGS
#endif

// C4514: unreferenced inline function has been removed (/Wall)
// C4820: '<typename>' : 'N' bytes padding added after data member (/Wall)
#ifndef _VCRUNTIME_DISABLED_WARNINGS
    #define _VCRUNTIME_DISABLED_WARNINGS _VCRUNTIME_DISABLED_WARNING_4339 _VCRUNTIME_DISABLED_WARNING_4412 4514 4820 _VCRUNTIME_EXTRA_DISABLED_WARNINGS
#endif

#pragma warning(push)
#pragma warning(disable: _VCRUNTIME_DISABLED_WARNINGS)
using va_list = char*;
#ifdef __cplusplus
extern "C" {
#endif

#if defined _M_X64

    void __cdecl __va_start(va_list* , ...);

    #define __crt_va_start_a(ap, x) ((void)(__va_start(&ap, x)))
    #define __crt_va_arg(ap, t)                                               \
        ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
            ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))             \
            :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
    #define __crt_va_end(ap)        ((void)(ap = (va_list)0))

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#if defined __cplusplus && !defined _CRT_NO_VA_START_VALIDATION
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

    #define __crt_va_start(ap, x) ((void)(__vcrt_assert_va_start_is_not_reference<decltype(x)>(), __crt_va_start_a(ap, x)))

#else // ^^^ __cplusplus ^^^ // vvv !__cplusplus vvv //

    #define __crt_va_start(ap, x) __crt_va_start_a(ap, x)

#endif

#pragma warning(pop) // _VCRUNTIME_DISABLED_WARNINGS
#pragma pack(pop)
