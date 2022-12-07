export module sl.numbers;

export namespace std::numbers
{
	template <class T>
	struct _Invalid {
		static_assert(_Always_false<T>, "A program that instantiates a primary template of a mathematical constant "
			"variable template is ill-formed. (N4835 [math.constants]/3)");
	};

	template <class T>
	inline constexpr T e_v = _Invalid<T>{};
	template <class T>
	inline constexpr T log2e_v = _Invalid<T>{};
	template <class T>
	inline constexpr T log10e_v = _Invalid<T>{};
	template <class T>
	inline constexpr T pi_v = _Invalid<T>{};
	template <class T>
	inline constexpr T inv_pi_v = _Invalid<T>{};
	template <class T>
	inline constexpr T inv_sqrtpi_v = _Invalid<T>{};
	template <class T>
	inline constexpr T ln2_v = _Invalid<T>{};
	template <class T>
	inline constexpr T ln10_v = _Invalid<T>{};
	template <class T>
	inline constexpr T sqrt2_v = _Invalid<T>{};
	template <class T>
	inline constexpr T sqrt3_v = _Invalid<T>{};
	template <class T>
	inline constexpr T inv_sqrt3_v = _Invalid<T>{};
	template <class T>
	inline constexpr T egamma_v = _Invalid<T>{};
	template <class T>
	inline constexpr T phi_v = _Invalid<T>{};

	template <>
	inline constexpr double e_v<double> = 2.718281828459045;
	template <>
	inline constexpr double log2e_v<double> = 1.4426950408889634;
	template <>
	inline constexpr double log10e_v<double> = 0.4342944819032518;
	template <>
	inline constexpr double pi_v<double> = 3.141592653589793;
	template <>
	inline constexpr double inv_pi_v<double> = 0.3183098861837907;
	template <>
	inline constexpr double inv_sqrtpi_v<double> = 0.5641895835477563;
	template <>
	inline constexpr double ln2_v<double> = 0.6931471805599453;
	template <>
	inline constexpr double ln10_v<double> = 2.302585092994046;
	template <>
	inline constexpr double sqrt2_v<double> = 1.4142135623730951;
	template <>
	inline constexpr double sqrt3_v<double> = 1.7320508075688772;
	template <>
	inline constexpr double inv_sqrt3_v<double> = 0.5773502691896257;
	template <>
	inline constexpr double egamma_v<double> = 0.5772156649015329;
	template <>
	inline constexpr double phi_v<double> = 1.618033988749895;

	template <>
	inline constexpr float e_v<float> = static_cast<float>(e_v<double>);
	template <>
	inline constexpr float log2e_v<float> = static_cast<float>(log2e_v<double>);
	template <>
	inline constexpr float log10e_v<float> = static_cast<float>(log10e_v<double>);
	template <>
	inline constexpr float pi_v<float> = static_cast<float>(pi_v<double>);
	template <>
	inline constexpr float inv_pi_v<float> = static_cast<float>(inv_pi_v<double>);
	template <>
	inline constexpr float inv_sqrtpi_v<float> = static_cast<float>(inv_sqrtpi_v<double>);
	template <>
	inline constexpr float ln2_v<float> = static_cast<float>(ln2_v<double>);
	template <>
	inline constexpr float ln10_v<float> = static_cast<float>(ln10_v<double>);
	template <>
	inline constexpr float sqrt2_v<float> = static_cast<float>(sqrt2_v<double>);
	template <>
	inline constexpr float sqrt3_v<float> = static_cast<float>(sqrt3_v<double>);
	template <>
	inline constexpr float inv_sqrt3_v<float> = static_cast<float>(inv_sqrt3_v<double>);
	template <>
	inline constexpr float egamma_v<float> = static_cast<float>(egamma_v<double>);
	template <>
	inline constexpr float phi_v<float> = static_cast<float>(phi_v<double>);

	inline constexpr double e = e_v<double>;
	inline constexpr double log2e = log2e_v<double>;
	inline constexpr double log10e = log10e_v<double>;
	inline constexpr double pi = pi_v<double>;
	inline constexpr double inv_pi = inv_pi_v<double>;
	inline constexpr double inv_sqrtpi = inv_sqrtpi_v<double>;
	inline constexpr double ln2 = ln2_v<double>;
	inline constexpr double ln10 = ln10_v<double>;
	inline constexpr double sqrt2 = sqrt2_v<double>;
	inline constexpr double sqrt3 = sqrt3_v<double>;
	inline constexpr double inv_sqrt3 = inv_sqrt3_v<double>;
	inline constexpr double egamma = egamma_v<double>;
	inline constexpr double phi = phi_v<double>;
}