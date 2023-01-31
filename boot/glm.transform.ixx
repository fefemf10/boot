export module glm.transform;
import sl.math;
export import glm.setup;
export import glm.geometric;
export import glm.mat4x4;
export namespace glm
{
	template<class T> mat<4, 4, T> translate(const mat<4, 4, T>& m, const vec<3, T>& v)
	{
		mat<4, 4, T> result(m);
		result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return result;
	}
	template<class T> mat<4, 4, T> rotate(const mat<4, 4, T>& m, T angle, const vec<3, T>& v)
	{
		T a = angle;
		T c = std::cos(a);
		T s = std::sin(a);

		vec<3, T> axis(normalize(v));
		vec<3, T> temp((T(1) - c) * axis);

		mat<4, 4, T> Rotate;
		Rotate[0][0] = c + temp[0] * axis[0];
		Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		Rotate[1][1] = c + temp[1] * axis[1];
		Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		Rotate[2][2] = c + temp[2] * axis[2];

		mat<4, 4, T> result;
		result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		result[3] = m[3];
		return result;
	}
	template<class T> mat<4, 4, T> scale(const mat<4, 4, T>& m, const vec<3, T>& v)
	{
		mat<4, 4, T> result;
		result[0] = m[0] * v[0];
		result[1] = m[1] * v[1];
		result[2] = m[2] * v[2];
		result[3] = m[3];
		return result;
	}
	template<class T> mat<4, 4, T> lookAtRH(const vec<3, T>& eye, const vec<3, T>& center, const vec<3, T>& up)
	{
		vec<3, T> f(normalize(center - eye));
		vec<3, T> s(normalize(cross(f, up)));
		vec<3, T> u(cross(s, f));

		mat<4, 4, T> result(1);
		result[0][0] = s.x;
		result[1][0] = s.y;
		result[2][0] = s.z;
		result[0][1] = u.x;
		result[1][1] = u.y;
		result[2][1] = u.z;
		result[0][2] = -f.x;
		result[1][2] = -f.y;
		result[2][2] = -f.z;
		result[3][0] = -dot(s, eye);
		result[3][1] = -dot(u, eye);
		result[3][2] = dot(f, eye);
		return result;
	}
	template<class T> mat<4, 4, T> lookAtLH(const vec<3, T>& eye, const vec<3, T>& center, const vec<3, T>& up)
	{
		vec<3, T> f(normalize(center - eye));
		vec<3, T> s(normalize(cross(up, f)));
		vec<3, T> u(cross(f, s));

		mat<4, 4, T> result(1);
		result[0][0] = s.x;
		result[1][0] = s.y;
		result[2][0] = s.z;
		result[0][1] = u.x;
		result[1][1] = u.y;
		result[2][1] = u.z;
		result[0][2] = f.x;
		result[1][2] = f.y;
		result[2][2] = f.z;
		result[3][0] = -dot(s, eye);
		result[3][1] = -dot(u, eye);
		result[3][2] = -dot(f, eye);
		return result;
	}
	template<class T> mat<4, 4, T> lookAt(const vec<3, T>& eye, const vec<3, T>& center, const vec<3, T>& up)
	{
		if constexpr (GLM_CONFIG_CLIP_CONTROL & GLM_CLIP_CONTROL_LH_BIT)
			return lookAtLH(eye, center, up);
		else
			return lookAtRH(eye, center, up);
	}
}