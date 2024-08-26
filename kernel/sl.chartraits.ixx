export module sl.chartraits;
import sl.compare;
import sl.bit;
import sl.utility;
import sl.algorithm;
import sl.concepts;
import sl.compat;

namespace std
{
	template <character T1, integral T2>
	struct internal_char_traits
	{
		using char_type = T1;
		using int_type = T2;
		using comparison_category = strong_ordering;

		static constexpr char_type* copy(char_type* const dst, const char_type* const src, const size_t size) noexcept
		{
			if (is_constant_evaluated())
			{
				for (size_t i = 0; i != size; ++i)
					dst[i] = src[i];
				return dst;
			}
			::memcpy(dst, src, size * sizeof(char_type));
			return dst;
		}

		static constexpr char_type* move(char_type* const dst, const char_type* const src, const size_t size) noexcept
		{
			if (is_constant_evaluated())
			{
				bool _Loop_forward = true;

				for (const char_type* _Src = src; _Src != src + size; ++_Src)
					if (dst == _Src)
					{
						_Loop_forward = false;
						break;
					}

				if (_Loop_forward)
					for (size_t i = 0; i != size; ++i)
						dst[i] = src[i];
				else
					for (size_t i = size; i != 0; --i)
						dst[i - 1] = src[i - 1];
				return dst;
			}
			::memmove(dst, src, size * sizeof(char_type));
			return dst;
		}

		[[nodiscard]] static constexpr int compare(const char_type* dst, const char_type* src, size_t size) noexcept
		{
			for (; 0 < size; --size, ++dst, ++src)
				if (*dst != *src)
					return *dst < *src ? -1 : +1;
			return 0;
		}

		[[nodiscard]] static constexpr size_t length(const char_type* src) noexcept
		{
			size_t size = 0;
			while (*src != char_type())
			{
				++size;
				++src;
			}
			return size;
		}

		[[nodiscard]] static constexpr const char_type* find(const char_type* src, size_t size, const char_type& c) noexcept
		{
			for (; 0 < size; --size, ++src)
				if (*src == c)
					return src;
			return nullptr;
		}

		static constexpr char_type* assign(const char_type* src, size_t size, const char_type c) noexcept
		{
			for (char_type* next = src; size > 0; --size, ++next)
				*next = c;
			return src;
		}

		static constexpr void assign(char_type& lhs, const char_type& rhs) noexcept
		{
			lhs = rhs;
		}

		[[nodiscard]] static constexpr bool eq(const char_type& lhs, const char_type& rhs) noexcept { return lhs == rhs; }
		[[nodiscard]] static constexpr bool lt(const char_type& lhs, const char_type& rhs) noexcept { return lhs < rhs; }
		[[nodiscard]] static constexpr char_type to_char_type(const int_type& int_type) noexcept { return static_cast<char_type>(int_type); }
		[[nodiscard]] static constexpr int_type to_int_type(const char_type& c) noexcept { return static_cast<int_type>(c); }
		[[nodiscard]] static constexpr bool eq_int_type(const int_type& lhs, const int_type& rhs) noexcept { return lhs == rhs; }
		[[nodiscard]] static constexpr int_type not_eof(const int_type& int_type) noexcept { return int_type != eof() ? int_type : !eof(); }
		[[nodiscard]] static constexpr int_type eof() noexcept { return static_cast<int_type>(-1); }
	};



	template <class _Traits>
	using _Traits_ch_t = typename _Traits::char_type;

	template <class _Traits>
	using _Traits_ptr_t = const typename _Traits::char_type*;

	template <class _Traits>
	constexpr bool _Traits_equal(const _Traits_ptr_t<_Traits> left, const size_t _Left_size, const _Traits_ptr_t<_Traits> right, const size_t _Right_size) noexcept
	{
		return _Left_size == _Right_size && _Traits::compare(left, right, _Left_size) == 0;
	}

	template <class _Traits>
	constexpr int _Traits_compare(const _Traits_ptr_t<_Traits> left, const size_t _Left_size, const _Traits_ptr_t<_Traits> right, const size_t _Right_size) noexcept
	{
		const int _Ans = _Traits::compare(left, right, min(_Left_size, _Right_size));

		if (_Ans != 0) {
			return _Ans;
		}

		if (_Left_size < _Right_size) {
			return -1;
		}

		if (_Left_size > _Right_size) {
			return 1;
		}

		return 0;
	}

	template <class _Traits>
	constexpr size_t _Traits_find(const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size, const size_t _Start_at, const _Traits_ptr_t<_Traits> _Needle, const size_t _Needle_size) noexcept
	{
		if (_Needle_size > _Hay_size || _Start_at > _Hay_size - _Needle_size)
		{
			return static_cast<size_t>(-1);
		}

		if (_Needle_size == 0)
		{
			return _Start_at;
		}

		const auto _Possible_matches_end = _Haystack + (_Hay_size - _Needle_size) + 1;
		for (auto _Match_try = _Haystack + _Start_at;; ++_Match_try)
		{
			_Match_try = _Traits::find(_Match_try, static_cast<size_t>(_Possible_matches_end - _Match_try), *_Needle);
			if (!_Match_try)
			{
				return static_cast<size_t>(-1);
			}

			if (_Traits::compare(_Match_try, _Needle, _Needle_size) == 0)
			{
				return static_cast<size_t>(_Match_try - _Haystack);
			}
		}
	}

	template <class _Traits>
	constexpr size_t _Traits_find_ch(const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size, const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept
	{
		if (_Start_at < _Hay_size)
		{
			const auto _Found_at = _Traits::find(_Haystack + _Start_at, _Hay_size - _Start_at, _Ch);
			if (_Found_at)
			{
				return static_cast<size_t>(_Found_at - _Haystack);
			}
		}

		return static_cast<size_t>(-1);
	}

	template <class _Traits>
	constexpr size_t _Traits_rfind(const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size, const size_t _Start_at, const _Traits_ptr_t<_Traits> _Needle, const size_t _Needle_size) noexcept
	{
		if (_Needle_size == 0)
		{
			return min(_Start_at, _Hay_size);
		}

		if (_Needle_size <= _Hay_size)
		{
			for (auto _Match_try = _Haystack + min(_Start_at, _Hay_size - _Needle_size);; --_Match_try)
			{
				if (_Traits::eq(*_Match_try, *_Needle) && _Traits::compare(_Match_try, _Needle, _Needle_size) == 0)
				{
					return static_cast<size_t>(_Match_try - _Haystack);
				}

				if (_Match_try == _Haystack) {
					break;
				}
			}
		}

		return static_cast<size_t>(-1);
	}

	template <class _Traits>
	constexpr size_t _Traits_rfind_ch(const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size, const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept
	{
		if (_Hay_size != 0)
		{
			for (auto _Match_try = _Haystack + min(_Start_at, _Hay_size - 1);; --_Match_try)
			{
				if (_Traits::eq(*_Match_try, _Ch))
				{
					return static_cast<size_t>(_Match_try - _Haystack);
				}

				if (_Match_try == _Haystack)
				{
					break;
				}
			}
		}

		return static_cast<size_t>(-1);
	}

	template <character T1, integral T2>
	struct narrow_char_traits : public internal_char_traits<T1, T2>
	{
	public:
		using char_type = T1;
		using int_type = T2;
		using comparison_category = strong_ordering;
		[[nodiscard]] static constexpr int compare(const char_type* dst, const char_type* src, size_t size) noexcept
		{
			return __builtin_memcmp(dst, src, size);
		}
		[[nodiscard]] static constexpr size_t length(const char_type* src) noexcept
		{
			return __builtin_strlen(src);
		}
		[[nodiscard]] static constexpr const char_type* find(const char_type* src, size_t size, const char_type& c) noexcept
		{
			return __builtin_char_memchr(src, c, size);
		}
		static constexpr char_type* assign(const char_type* src, size_t size, const char_type c) noexcept
		{
			return static_cast<char_type*>(::memset(src, c, size));
		}
	};
}

export namespace std
{
	template <character T> struct char_traits : public internal_char_traits<T, int> {};
	template <> struct char_traits<char> : narrow_char_traits<char, int> {};
	template <> struct char_traits<char8_t> : internal_char_traits<char8_t, unsigned int> {};
	template <> struct char_traits<char16_t> : internal_char_traits<char16_t, unsigned short> {};
	template <> struct char_traits<char32_t> : internal_char_traits<char32_t, unsigned int> {};
}