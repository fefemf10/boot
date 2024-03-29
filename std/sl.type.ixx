export module sl.type;

export namespace std
{
	using int8_t = char;
	using int16_t = short;
	using int32_t = int;
	using int64_t = long long;
	using ssize_t = unsigned long long;
	using uint8_t = unsigned char;
	using uint16_t = unsigned short;
	using uint32_t = unsigned int;
	using uint64_t = unsigned long long;
	using ptrdiff_t = long long;
	using size_t = unsigned long long;
	using nullptr_t = decltype(nullptr);
}