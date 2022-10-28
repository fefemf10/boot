#pragma once
#define _INC_STDARG
#include "vadefs.h"

__pragma(pack(push, 8))
extern "C" {
#define va_start __crt_va_start
#define va_arg   __crt_va_arg
#define va_end   __crt_va_end
#define va_copy(destination, source) ((destination) = (source))
}
__pragma(pack(pop))
