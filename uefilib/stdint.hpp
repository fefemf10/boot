#pragma once
using int8_t = signed char;
using int16_t = short;
using int32_t = int;
using int64_t = long long;
using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;
using int_least8_t = signed char;
using int_least16_t = short;
using int_least32_t = int;
using int_least64_t = long long;
using uint_least8_t = unsigned char;
using uint_least16_t = unsigned short;
using uint_least32_t = unsigned int;
using uint_least64_t = unsigned long long;
using int_fast8_t = signed char;
using int_fast16_t = int;
using int_fast32_t = int;
using int_fast64_t = long long;
using uint_fast8_t = unsigned char;
using uint_fast16_t = unsigned int;
using uint_fast32_t = unsigned int;
using uint_fast64_t = unsigned long long;
using intmax_t = long long;
using uintmax_t = unsigned long long;
using uintptr_t = uint64_t;

constexpr auto INT8_MIN = (-127i8 - 1);
constexpr auto INT16_MIN = (-32767i16 - 1);
constexpr auto INT32_MIN = (-2147483647i32 - 1);
constexpr auto INT64_MIN = (-9223372036854775807i64 - 1);
constexpr auto INT8_MAX = 127i8;
constexpr auto INT16_MAX = 32767i16;
constexpr auto INT32_MAX = 2147483647i32;
constexpr auto INT64_MAX = 9223372036854775807i64;
constexpr auto UINT8_MAX = 0xffui8;
constexpr auto UINT16_MAX = 0xffffui16;
constexpr auto UINT32_MAX = 0xffffffffui32;
constexpr auto UINT64_MAX = 0xffffffffffffffffui64;