export module glm.mat;
import types;
export import glm.vec;
export namespace glm
{
	template<size_t C, size_t R, typename T> struct mat;
	using f32mat2x2 = mat<2, 2, f32>;
	using f32mat2x3 = mat<2, 3, f32>;
	using f32mat2x4 = mat<2, 4, f32>;
	using f32mat3x2 = mat<3, 2, f32>;
	using f32mat3x3 = mat<3, 3, f32>;
	using f32mat3x4 = mat<3, 4, f32>;
	using f32mat4x2 = mat<4, 2, f32>;
	using f32mat4x3 = mat<4, 3, f32>;
	using f32mat4x4 = mat<4, 4, f32>;
	using f64mat2x2 = mat<2, 2, f64>;
	using f64mat2x3 = mat<2, 3, f64>;
	using f64mat2x4 = mat<2, 4, f64>;
	using f64mat3x2 = mat<3, 2, f64>;
	using f64mat3x3 = mat<3, 3, f64>;
	using f64mat3x4 = mat<3, 4, f64>;
	using f64mat4x2 = mat<4, 2, f64>;
	using f64mat4x3 = mat<4, 3, f64>;
	using f64mat4x4 = mat<4, 4, f64>;
	using i8mat2x2 = mat<2, 2, i8>;
	using i8mat2x3 = mat<2, 3, i8>;
	using i8mat2x4 = mat<2, 4, i8>;
	using i8mat3x2 = mat<3, 2, i8>;
	using i8mat3x3 = mat<3, 3, i8>;
	using i8mat3x4 = mat<3, 4, i8>;
	using i8mat4x2 = mat<4, 2, i8>;
	using i8mat4x3 = mat<4, 3, i8>;
	using i8mat4x4 = mat<4, 4, i8>;
	using u8mat2x2 = mat<2, 2, u8>;
	using u8mat2x3 = mat<2, 3, u8>;
	using u8mat2x4 = mat<2, 4, u8>;
	using u8mat3x2 = mat<3, 2, u8>;
	using u8mat3x3 = mat<3, 3, u8>;
	using u8mat3x4 = mat<3, 4, u8>;
	using u8mat4x2 = mat<4, 2, u8>;
	using u8mat4x3 = mat<4, 3, u8>;
	using u8mat4x4 = mat<4, 4, u8>;
	using i16mat2x2 = mat<2, 2, i16>;
	using i16mat2x3 = mat<2, 3, i16>;
	using i16mat2x4 = mat<2, 4, i16>;
	using i16mat3x2 = mat<3, 2, i16>;
	using i16mat3x3 = mat<3, 3, i16>;
	using i16mat3x4 = mat<3, 4, i16>;
	using i16mat4x2 = mat<4, 2, i16>;
	using i16mat4x3 = mat<4, 3, i16>;
	using i16mat4x4 = mat<4, 4, i16>;
	using u16mat2x2 = mat<2, 2, u16>;
	using u16mat2x3 = mat<2, 3, u16>;
	using u16mat2x4 = mat<2, 4, u16>;
	using u16mat3x2 = mat<3, 2, u16>;
	using u16mat3x3 = mat<3, 3, u16>;
	using u16mat3x4 = mat<3, 4, u16>;
	using u16mat4x2 = mat<4, 2, u16>;
	using u16mat4x3 = mat<4, 3, u16>;
	using u16mat4x4 = mat<4, 4, u16>;
	using i32mat2x2 = mat<2, 2, i32>;
	using i32mat2x3 = mat<2, 3, i32>;
	using i32mat2x4 = mat<2, 4, i32>;
	using i32mat3x2 = mat<3, 2, i32>;
	using i32mat3x3 = mat<3, 3, i32>;
	using i32mat3x4 = mat<3, 4, i32>;
	using i32mat4x2 = mat<4, 2, i32>;
	using i32mat4x3 = mat<4, 3, i32>;
	using i32mat4x4 = mat<4, 4, i32>;
	using u32mat2x2 = mat<2, 2, u32>;
	using u32mat2x3 = mat<2, 3, u32>;
	using u32mat2x4 = mat<2, 4, u32>;
	using u32mat3x2 = mat<3, 2, u32>;
	using u32mat3x3 = mat<3, 3, u32>;
	using u32mat3x4 = mat<3, 4, u32>;
	using u32mat4x2 = mat<4, 2, u32>;
	using u32mat4x3 = mat<4, 3, u32>;
	using u32mat4x4 = mat<4, 4, u32>;
	using i64mat2x2 = mat<2, 2, i64>;
	using i64mat2x3 = mat<2, 3, i64>;
	using i64mat2x4 = mat<2, 4, i64>;
	using i64mat3x2 = mat<3, 2, i64>;
	using i64mat3x3 = mat<3, 3, i64>;
	using i64mat3x4 = mat<3, 4, i64>;
	using i64mat4x2 = mat<4, 2, i64>;
	using i64mat4x3 = mat<4, 3, i64>;
	using i64mat4x4 = mat<4, 4, i64>;
	using u64mat2x2 = mat<2, 2, u64>;
	using u64mat2x3 = mat<2, 3, u64>;
	using u64mat2x4 = mat<2, 4, u64>;
	using u64mat3x2 = mat<3, 2, u64>;
	using u64mat3x3 = mat<3, 3, u64>;
	using u64mat3x4 = mat<3, 4, u64>;
	using u64mat4x2 = mat<4, 2, u64>;
	using u64mat4x3 = mat<4, 3, u64>;
	using u64mat4x4 = mat<4, 4, u64>;

	

	/*template<typename T>
	struct mat<2, 3, T>
	{
		using col_type = vec<3, T>;
		using row_type = vec<2, T>;
		using type = mat<2, 3, T>;
		using transpose_type = mat<3, 2, T>;
		using value_type = T;
		constexpr static size_t length() { return 2; }
		col_type& operator[](size_t i) { return value[i]; }
		constexpr const col_type& operator[](size_t i) const { return value[i]; }
	private:
		col_type value[2];
	};

	template<typename T>
	struct mat<2, 4, T>
	{
		using col_type = vec<4, T>;
		using row_type = vec<2, T>;
		using type = mat<2, 4, T>;
		using transpose_type = mat<4, 2, T>;
		using value_type = T;
		constexpr static size_t length() { return 2; }
		col_type& operator[](size_t i) { return value[i]; }
		constexpr const col_type& operator[](size_t i) const { return value[i]; }
	private:
		col_type value[2];
	};*/
}