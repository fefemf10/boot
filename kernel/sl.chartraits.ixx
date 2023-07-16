export module sl.chartraits;
import sl.compare;
import sl.bit;
import sl.utility;
import sl.memory;
import sl.algorithm;
export namespace std
{
	template <class T, class _Int_type>
	struct _Char_traits
	{
		using char_type = T;
		using int_type = _Int_type;
		using comparison_category = strong_ordering;

		static constexpr T* copy(T* const _First1, const T* const _First2, const size_t _Count) noexcept
		{
			if (is_constant_evaluated())
			{
				for (size_t _Idx = 0; _Idx != _Count; ++_Idx)
				{
					_First1[_Idx] = _First2[_Idx];
				}

				return _First1;
			}

			memcpy(_First1, _First2, _Count * sizeof(T));

			return _First1;
		}

		static constexpr T* move(T* const _First1, const T* const _First2, const size_t _Count) noexcept
		{
			if (is_constant_evaluated())
			{
				bool _Loop_forward = true;

				for (const T* _Src = _First2; _Src != _First2 + _Count; ++_Src)
				{
					if (_First1 == _Src)
					{
						_Loop_forward = false;
						break;
					}
				}

				if (_Loop_forward)
				{
					for (size_t _Idx = 0; _Idx != _Count; ++_Idx)
					{
						_First1[_Idx] = _First2[_Idx];
					}
				}
				else
				{
					for (size_t _Idx = _Count; _Idx != 0; --_Idx)
					{
						_First1[_Idx - 1] = _First2[_Idx - 1];
					}
				}

				return _First1;
			}

			memmove(_First1, _First2, _Count * sizeof(T));

			return _First1;
		}


		[[nodiscard]] static constexpr int compare(const T* _First1, const T* _First2, size_t _Count) noexcept
		{
			for (; 0 < _Count; --_Count, ++_First1, ++_First2)
			{
				if (*_First1 != *_First2)
				{
					return *_First1 < *_First2 ? -1 : +1;
				}
			}

			return 0;
		}

		[[nodiscard]] static constexpr size_t length(const T* _First) noexcept
		{
			size_t _Count = 0;
			while (*_First != T())
			{
				++_Count;
				++_First;
			}

			return _Count;
		}

		[[nodiscard]] static constexpr const T* find(const T* _First, size_t _Count, const T& _Ch) noexcept
		{
			for (; 0 < _Count; --_Count, ++_First)
			{
				if (*_First == _Ch)
				{
					return _First;
				}
			}

			return nullptr;
		}

		static constexpr T* assign(T* const _First, size_t _Count, const T _Ch) noexcept
		{
			if (is_constant_evaluated())
			{
				for (T* _Next = _First; _Count > 0; --_Count, ++_Next)
				{
					construct_at(_Next, _Ch);
				}
			}
			else
			{
				for (T* _Next = _First; _Count > 0; --_Count, ++_Next)
				{
					*_Next = _Ch;
				}
			}

			return _First;
		}

		static constexpr void assign(T& _Left, const T& _Right) noexcept
		{
			if (is_constant_evaluated())
			{
				construct_at(addressof(_Left), _Right);
			}
			else
			{
				_Left = _Right;
			}
		}

		[[nodiscard]] static constexpr bool eq(const T& _Left, const T& _Right) noexcept
		{
			return _Left == _Right;
		}

		[[nodiscard]] static constexpr bool lt(const T& _Left, const T& _Right) noexcept
		{
			return _Left < _Right;
		}

		[[nodiscard]] static constexpr T to_char_type(const int_type& _Meta) noexcept
		{
			return static_cast<T>(_Meta);
		}

		[[nodiscard]] static constexpr int_type to_int_type(const T& _Ch) noexcept
		{
			return static_cast<int_type>(_Ch);
		}

		[[nodiscard]] static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept
		{
			return _Left == _Right;
		}

		[[nodiscard]] static constexpr int_type not_eof(const int_type& _Meta) noexcept
		{
			return _Meta != eof() ? _Meta : !eof();
		}

		[[nodiscard]] static constexpr int_type eof() noexcept
		{
			return static_cast<int_type>(-1);
		}
	};

	template <class T, class _Int_type>
	struct _Narrow_char_traits : private _Char_traits<T, _Int_type> {
	private:
		using _Primary_char_traits = _Char_traits<T, _Int_type>;

	public:
		using char_type = T;
		using int_type = _Int_type;
		using comparison_category = strong_ordering;

		using _Primary_char_traits::copy;
		using _Primary_char_traits::move;

		[[nodiscard]] static constexpr int compare(const T* const _First1, const T* const _First2, const size_t _Count) noexcept
		{
			return __builtin_memcmp(_First1, _First2, _Count);
		}

		[[nodiscard]] static constexpr size_t length(const T* const _First) noexcept
		{
			if constexpr (is_same_v<T, char8_t>) 
			{
				return __builtin_u8strlen(_First);
			}
			else
			{
				return __builtin_strlen(_First);
			}
		}

		[[nodiscard]] static constexpr const T* find(const T* const _First, const size_t _Count, const T& _Ch) noexcept
		{
			if constexpr (is_same_v<T, char8_t>) 
			{
				return __builtin_u8memchr(_First, _Ch, _Count);
			}
			else
			{
				return __builtin_char_memchr(_First, _Ch, _Count);
			}
		}

		static constexpr T* assign(T* const _First, size_t _Count, const T _Ch) noexcept
		{
			if (is_constant_evaluated())
			{
				return _Primary_char_traits::assign(_First, _Count, _Ch);
			}
			return static_cast<T*>(memset(_First, _Ch, _Count));
		}

		static constexpr void assign(T& _Left, const T& _Right) noexcept
		{
			if (is_constant_evaluated())
			{
				return _Primary_char_traits::assign(_Left, _Right);
			}
			_Left = _Right;
		}

		[[nodiscard]] static constexpr bool eq(const T& _Left, const T& _Right) noexcept
		{
			return _Left == _Right;
		}

		[[nodiscard]] static constexpr bool lt(const T& _Left, const T& _Right) noexcept
		{
			return static_cast<unsigned char>(_Left) < static_cast<unsigned char>(_Right);
		}

		[[nodiscard]] static constexpr T to_char_type(const int_type& _Meta) noexcept
		{
			return static_cast<T>(_Meta);
		}

		[[nodiscard]] static constexpr int_type to_int_type(const T& _Ch) noexcept
		{
			return static_cast<unsigned char>(_Ch);
		}

		[[nodiscard]] static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept
		{
			return _Left == _Right;
		}

		[[nodiscard]] static constexpr int_type not_eof(const int_type& _Meta) noexcept
		{
			return _Meta != eof() ? _Meta : !eof();
		}

		[[nodiscard]] static constexpr int_type eof() noexcept
		{
			return static_cast<int_type>(-1);
		}
	};

	template <class T> struct char_traits : _Char_traits<T, long> {};
	template <> struct char_traits<char> : _Narrow_char_traits<char, unsigned int> {};
	template <> struct char_traits<char8_t> : _Narrow_char_traits<char8_t, unsigned int> {};
	template <> struct char_traits<char16_t> : _Char_traits<char16_t, unsigned short> {};
	template <> struct char_traits<char32_t> : _Char_traits<char32_t, unsigned int> {};

	template <class _Traits>
	using _Traits_ch_t = typename _Traits::char_type;

	template <class _Traits>
	using _Traits_ptr_t = const typename _Traits::char_type*;

	template <class _Traits>
	constexpr bool _Traits_equal(const _Traits_ptr_t<_Traits> _Left, const size_t _Left_size, const _Traits_ptr_t<_Traits> _Right, const size_t _Right_size) noexcept
	{
		return _Left_size == _Right_size && _Traits::compare(_Left, _Right, _Left_size) == 0;
	}

	template <class _Traits>
	constexpr int _Traits_compare(const _Traits_ptr_t<_Traits> _Left, const size_t _Left_size, const _Traits_ptr_t<_Traits> _Right, const size_t _Right_size) noexcept
	{
		const int _Ans = _Traits::compare(_Left, _Right, min(_Left_size, _Right_size));

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

	template <class _Elem, bool = _Is_character<_Elem>::value>
	class _String_bitmap
	{
	public:
		constexpr bool _Mark(const _Elem* _First, const _Elem* const _Last) noexcept
		{
			for (; _First != _Last; ++_First)
			{
				_Matches[static_cast<unsigned char>(*_First)] = true;
			}

			return true;
		}

		constexpr bool _Match(const _Elem _Ch) const noexcept
		{
			return _Matches[static_cast<unsigned char>(_Ch)];
		}

	private:
		bool _Matches[256] = {};
	};
}