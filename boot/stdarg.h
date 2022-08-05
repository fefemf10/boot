//
// stdarg.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The C Standard Library <stdarg.h> header.
//
#pragma once
#define _INC_STDARG
#include "vadefs.h"

#pragma warning(push)
#pragma warning(disable: _VCRUNTIME_DISABLED_WARNINGS)

__pragma(pack(push, _CRT_PACKING))
extern "C" {



#define va_start __crt_va_start
#define va_arg   __crt_va_arg
#define va_end   __crt_va_end
#define va_copy(destination, source) ((destination) = (source))



}
__pragma(pack(pop))
#pragma warning(pop) // _VCRUNTIME_DISABLED_WARNINGS
