export module math;
import types;
export namespace math
{
	constexpr u16 byteswap(u16 value);
	constexpr u32 byteswap(u32 value);
	constexpr u64 byteswap(u64 value);

	constexpr i32 abs(i32 value);
	constexpr i64 abs(i64 value);

	constexpr f32 lerp(f32 a, f32 b, f32 t);

	constexpr f32 exp(f32 value);
	constexpr f32 exp2(f32 value);
	constexpr f32 expml(f32 value);
	constexpr f32 log(f32 value);
	constexpr f32 log10(f32 value);
	constexpr f32 log2(f32 value);
	constexpr f32 loglp(f32 value);

	constexpr f32 pow(f32 value, f32 exp);
	constexpr f32 sqrt(f32 value);
	constexpr f32 cbrt(f32 value);
	constexpr f32 hypot(f32 x, f32 y);

	constexpr f32 abs(f32 value);
	constexpr f32 cos(f32 value);
	constexpr f32 sin(f32 value);
	constexpr f32 tan(f32 value);
	constexpr f32 acos(f32 value);
	constexpr f32 asin(f32 value);
	constexpr f32 atan(f32 value);
	constexpr f32 atan2(f32 value);

	constexpr f64 lerp(f64 a, f64 b, f64 t);

	constexpr f64 exp(f64 value);
	constexpr f64 exp2(f64 value);
	constexpr f64 expml(f64 value);
	constexpr f64 log(f64 value);
	constexpr f64 log10(f64 value);
	constexpr f64 log2(f64 value);
	constexpr f64 loglp(f64 value);

	constexpr f64 pow(f64 value, f64 exp);
	constexpr f64 sqrt(f64 value);
	constexpr f64 cbrt(f64 value);
	constexpr f64 hypot(f64 x, f64 y);

	constexpr f64 abs(f64 value);
	constexpr f64 cos(f64 value);
	constexpr f64 sin(f64 value);
	constexpr f64 tan(f64 value);
	constexpr f64 acos(f64 value);
	constexpr f64 asin(f64 value);
	constexpr f64 atan(f64 value);
	constexpr f64 atan2(f64 value);
}