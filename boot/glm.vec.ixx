export module glm.vec;
import types;

export namespace glm
{
	template<size_t L, typename T> struct vec;
	using f32vec1 = vec<1, f32>;
	using f32vec2 = vec<2, f32>;
	using f32vec3 = vec<3, f32>;
	using f32vec4 = vec<4, f32>;
	using f64vec1 = vec<1, f64>;
	using f64vec2 = vec<2, f64>;
	using f64vec3 = vec<3, f64>;
	using f64vec4 = vec<4, f64>;
	using i8vec1 = vec<1, i8>;
	using i8vec2 = vec<2, i8>;
	using i8vec3 = vec<3, i8>;
	using i8vec4 = vec<4, i8>;
	using u8vec1 = vec<1, u8>;
	using u8vec2 = vec<2, u8>;
	using u8vec3 = vec<3, u8>;
	using u8vec4 = vec<4, u8>;
	using i16vec1 = vec<1, i16>;
	using i16vec2 = vec<2, i16>;
	using i16vec3 = vec<3, i16>;
	using i16vec4 = vec<4, i16>;
	using u16vec1 = vec<1, u16>;
	using u16vec2 = vec<2, u16>;
	using u16vec3 = vec<3, u16>;
	using u16vec4 = vec<4, u16>;
	using i32vec1 = vec<1, i32>;
	using i32vec2 = vec<2, i32>;
	using i32vec3 = vec<3, i32>;
	using i32vec4 = vec<4, i32>;
	using u32vec1 = vec<1, u32>;
	using u32vec2 = vec<2, u32>;
	using u32vec3 = vec<3, u32>;
	using u32vec4 = vec<4, u32>;
	using i64vec1 = vec<1, i64>;
	using i64vec2 = vec<2, i64>;
	using i64vec3 = vec<3, i64>;
	using i64vec4 = vec<4, i64>;
	using u64vec1 = vec<1, u64>;
	using u64vec2 = vec<2, u64>;
	using u64vec3 = vec<3, u64>;
	using u64vec4 = vec<4, u64>;
}