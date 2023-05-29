export module sl.math;
import sl.type;
export namespace std
{
	constexpr uint16_t byteswap(uint16_t value);
	constexpr uint32_t byteswap(uint32_t value);
	constexpr uint64_t byteswap(uint64_t value);

	constexpr int32_t abs(int32_t value);
	constexpr int64_t abs(int64_t value);

	constexpr float lerp(float a, float b, float t);

	constexpr float exp(float value);
	constexpr float exp2(float value);
	constexpr float expml(float value);
	constexpr float log(float value);
	constexpr float log10(float value);
	constexpr float log2(float value);
	constexpr float loglp(float value);

	constexpr float pow(float value, float exp);
	constexpr float sqrt(float value);
	constexpr float cbrt(float value);
	constexpr float hypot(float x, float y);

	constexpr float abs(float value);
	constexpr float cos(float value);
	constexpr float sin(float value);
	constexpr float tan(float value);
	constexpr float acos(float value);
	constexpr float asin(float value);
	constexpr float atan(float value);
	constexpr float atan2(float value);

	constexpr double lerp(double a, double b, double t);

	constexpr double exp(double value);
	constexpr double exp2(double value);
	constexpr double expml(double value);
	constexpr double log(double value);
	constexpr double log10(double value);
	constexpr double log2(double value);
	constexpr double loglp(double value);

	constexpr double pow(double value, double exp);
	constexpr double sqrt(double value);
	constexpr double cbrt(double value);
	constexpr double hypot(double x, double y);

	constexpr double abs(double value);
	constexpr double cos(double value);
	constexpr double sin(double value);
	constexpr double tan(double value);
	constexpr double acos(double value);
	constexpr double asin(double value);
	constexpr double atan(double value);
	constexpr double atan2(double value);
}