export module sl.print;
import sl.concepts;
import sl.string_view;
import sl.type;
import sl.iterator_core;
export namespace std
{
	template <std::character T>
	struct _Decode_result
	{
		const T* nextPtr;
		bool _Is_unicode_scalar_value;
	};

	[[nodiscard]] constexpr _Decode_result<char> _Decode_utf(const char* first, const char* last, char32_t& value) noexcept {
		value = static_cast<char32_t>(static_cast<unsigned char>(*first));
		int _Num_bytes;
		if (value <= 0x7F)
			return { first + 1, true };
		else if (value >= 0xC2 && value <= 0xDF)
			_Num_bytes = 2;
		else if (value >= 0xE0 && value <= 0xEF)
			_Num_bytes = 3;
		else if (value >= 0xF0 && value <= 0xF4)
			_Num_bytes = 4;
		else
		{
			value = 0xFFFD;
			return { first + 1, false };
		}

		if (first + 1 == last) {
			value = 0xFFFD;
			return { last, false };
		}

		switch (value) {
		case 0xE0:
			if (static_cast<unsigned char>(first[1]) < 0xA0)
			{
				value = 0xFFFD;
				return { first + 1, false };
			}
			break;
		case 0xED:
			if (static_cast<unsigned char>(first[1]) > 0x9F)
			{
				value = 0xFFFD;
				return { first + 1, false };
			}
			break;
		case 0xF0:
			if (static_cast<unsigned char>(first[1]) < 0x90)
			{
				value = 0xFFFD;
				return { first + 1, false };
			}
			break;
		case 0xF4:
			if (static_cast<unsigned char>(first[1]) > 0x8F)
			{
				value = 0xFFFD;
				return { first + 1, false };
			}
			break;
		}

		switch (_Num_bytes) {
		case 2:
			value &= 0b1'1111u;
			break;
		case 3:
			value &= 0b1111u;
			break;
		case 4:
			value &= 0b111u;
			break;
		}

		for (int i = 1; i < _Num_bytes; ++i)
		{
			if (first + i >= last || static_cast<unsigned char>(first[i]) < 0x80 || static_cast<unsigned char>(first[i]) > 0xBF)
			{
				value = 0xFFFD;
				return { first + i, false };
			}
			value = (value << 6) | (static_cast<unsigned char>(first[i]) & 0b11'1111u);
		}
		return { first + _Num_bytes, true };
	}

	template <std::character T>
	class _Unicode_codepoint_iterator {
	private:
		const T* first = nullptr;
		const T* last = nullptr;
		const T* next = nullptr;
		char32_t value = 0;

	public:
		using value_type = char32_t;
		using difference_type = ptrdiff_t;

		constexpr _Unicode_codepoint_iterator(const string_view str) noexcept : first(str.cbegin()), last(str.cend())
		{
			next = _Decode_utf(first, last, value).nextPtr;
		}

		constexpr _Unicode_codepoint_iterator(const T* first, const T* last) noexcept: first(first), last(last)
		{
			next = _Decode_utf(first, last, value).nextPtr;
		}

		constexpr _Unicode_codepoint_iterator() = default;

		constexpr _Unicode_codepoint_iterator& operator++() noexcept
		{
			first = next;
			if (first != last)
			{
				next = _Decode_utf(first, last, value).nextPtr;
			}
			else
				value = 0;

			return *this;
		}
		constexpr _Unicode_codepoint_iterator operator++(int) noexcept
		{
			auto _Old = *this;
			++*this;
			return _Old;
		}

		[[nodiscard]] constexpr value_type operator*() const noexcept
		{
			return value;
		}

		[[nodiscard]] constexpr const T* _Position() const noexcept
		{
			return first;
		}

		[[nodiscard]] constexpr bool operator==(default_sentinel_t) const noexcept
		{
			return first == last;
		}
		[[nodiscard]] constexpr bool operator==(const _Unicode_codepoint_iterator& other) const noexcept
		{
			return first == other.first && last == other.last;
		}
	};
}