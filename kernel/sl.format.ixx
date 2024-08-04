export module sl.format;
import sl.type;
import sl.typetraits;
import sl.concepts;
import sl.bit;
export namespace std
{
	enum class _Fmt_align : uint8_t { _None, left, right, _Center };

	enum class _Fmt_sign : uint8_t { _None, _Plus, _Minus, _Space };

	enum class _Basic_format_arg_type : uint8_t {
		_None,
		_Int_type,
		_UInt_type,
		_Long_long_type,
		_ULong_long_type,
		_Bool_type,
		_Char_type,
		_Float_type,
		_Double_type,
		_Long_double_type,
		_Pointer_type,
		_CString_type,
		_String_type,
		_Custom_type,
	};
	static_assert(static_cast<int>(_Basic_format_arg_type::_Custom_type) < 16, "must fit in 4-bit bitfield");

	[[nodiscard]] constexpr bool _Is_integral_fmt_type(_Basic_format_arg_type T) {
		return T > _Basic_format_arg_type::_None && T <= _Basic_format_arg_type::_Char_type;
	}
	[[nodiscard]] constexpr bool _Is_arithmetic_fmt_type(_Basic_format_arg_type T) {
		return T > _Basic_format_arg_type::_None && T <= _Basic_format_arg_type::_Long_double_type;
	}

	struct _Auto_id_tag {};

	// clang-format off
	template <class T, class _CharT>
	concept _Parse_arg_id_callbacks = requires(T _At) {
		{ _At._On_auto_id() } -> same_as<void>;
		{ _At._On_manual_id(size_t{}) } -> same_as<void>;
	};

	template <class T, class _CharT>
	concept _Parse_replacement_field_callbacks = requires(T _At, const _CharT * _First, const _CharT * _Last) {
		{ _At._Parse_context };
		{ _At._On_text(_First, _Last) } -> same_as<void>;
		{ _At._On_replacement_field(size_t{}, static_cast<const _CharT*>(nullptr)) } -> same_as<void>;
		{ _At._On_format_specs(size_t{}, _First, _Last) } -> same_as<const _CharT*>;
	};

	/*template <class T, class _CharT>
	concept _Parse_align_callbacks = requires(T _At, basic_string_view<_CharT> _Sv, _Fmt_align _Aln) {
		{ _At._On_fill(_Sv) } -> same_as<void>;
		{ _At._On_align(_Aln) } -> same_as<void>;
	};*/

	template <class T, class _CharT>
	concept _Parse_width_callbacks = requires(T _At) {
		{ _At._On_width(int{}) } -> same_as<void>;
	};

	template <class T, class _CharT>
	concept _Parse_precision_callbacks = requires(T _At) {
		{ _At._On_precision(int{}) } -> same_as<void>;
	};

	template <class T, class _CharT>
	concept _Width_adapter_callbacks = requires(T _At) {
		{ _At._On_dynamic_width(_Auto_id_tag{}) } -> same_as<void>;
		{ _At._On_dynamic_width(size_t{}) } -> same_as<void>;
	};

	template <class T, class _CharT>
	concept _Precision_adapter_callbacks = requires(T _At) {
		{ _At._On_dynamic_precision(_Auto_id_tag{}) } -> same_as<void>;
		{ _At._On_dynamic_precision(size_t{}) } -> same_as<void>;
	};

	/*template <class T, class _CharT>
	concept _Parse_spec_callbacks = _Parse_align_callbacks<T, _CharT>
		&& _Parse_width_callbacks<T, _CharT>
		&& _Parse_precision_callbacks<T, _CharT>
		&& _Width_adapter_callbacks<T, _CharT>
		&& _Precision_adapter_callbacks<T, _CharT>
		&& requires(T _At, basic_string_view<_CharT> _Sv, _Fmt_align _Aln, _Fmt_sign _Sgn) {
			{ _At._On_sign(_Sgn) } -> same_as<void>;
			{ _At._On_hash() } -> same_as<void>;
			{ _At._On_zero() } -> same_as<void>;
			{ _At._On_localized() } -> same_as<void>;
			{ _At._On_type(_CharT{}) } -> same_as<void>;
	};*/
	// clang-format on

	template <class _CharT>
	struct _Decode_result {
		const _CharT* _Next_ptr;
		bool _Is_unicode_scalar_value; // Also _Is_usv below, see https://www.unicode.org/glossary/#unicode_scalar_value
		// _Is_unicode_scalar_value is also used for non-Unicode encodings, to indicate that the input can be converted to
		// Unicode.
	};


	[[nodiscard]] constexpr _Decode_result<char> _Decode_utf(const char* _First, const char* _Last, char32_t& _Val) noexcept {
		_Val = static_cast<char32_t>(static_cast<unsigned char>(*_First));
		int _Num_bytes;
		if (_Val <= 0x7F) {
			return { _First + 1, true };
		}
		else if (_Val >= 0xC2 && _Val <= 0xDF) {
			_Num_bytes = 2;
		}
		else if (_Val >= 0xE0 && _Val <= 0xEF) {
			_Num_bytes = 3;
		}
		else if (_Val >= 0xF0 && _Val <= 0xF4) {
			_Num_bytes = 4;
		}
		else {
			_Val = 0xFFFD;
			return { _First + 1, false };
		}

		if (_First + 1 == _Last) {
			_Val = 0xFFFD;
			return { _Last, false };
		}

		switch (_Val) {
		case 0xE0:
			if (static_cast<unsigned char>(_First[1]) < 0xA0) {
				_Val = 0xFFFD;
				return { _First + 1, false };
			}
			break;
		case 0xED:
			if (static_cast<unsigned char>(_First[1]) > 0x9F) {
				_Val = 0xFFFD;
				return { _First + 1, false };
			}
			break;
		case 0xF0:
			if (static_cast<unsigned char>(_First[1]) < 0x90) {
				_Val = 0xFFFD;
				return { _First + 1, false };
			}
			break;
		case 0xF4:
			if (static_cast<unsigned char>(_First[1]) > 0x8F) {
				_Val = 0xFFFD;
				return { _First + 1, false };
			}
			break;
		}
		switch (_Num_bytes) {
		case 2:
			_Val &= 0b1'1111u;
			break;
		case 3:
			_Val &= 0b1111u;
			break;
		case 4:
			_Val &= 0b111u;
			break;
		}
		for (int _Idx = 1; _Idx < _Num_bytes; ++_Idx) {
			if (_First + _Idx >= _Last || static_cast<unsigned char>(_First[_Idx]) < 0x80
				|| static_cast<unsigned char>(_First[_Idx]) > 0xBF) {
				_Val = 0xFFFD;
				return { _First + _Idx, false };
			}
			_Val = (_Val << 6) | (static_cast<unsigned char>(_First[_Idx]) & 0b11'1111u);
		}
		return { _First + _Num_bytes, true };
	}

	[[nodiscard]] constexpr _Decode_result<char32_t> _Decode_utf(
		const char32_t* _First, const char32_t* _Last, char32_t& _Val) noexcept {
		(void)_Last;
		_Val = *_First;
		const bool _Is_usv = _Val < 0xD800 || (_Val > 0xDFFF && _Val <= 0x10FFFF);
		return { _First + 1, _Is_usv };
	}





	template <class Context> class basic_format_args;

	template <class _Context, class... _Args>
	class _Format_arg_store {
	private:
		using _CharType = typename _Context::char_type;
		using _Traits = _Format_arg_traits<_Context>;

		friend basic_format_args<_Context>;

		static constexpr size_t _Num_args = sizeof...(_Args);
		static constexpr size_t _Storage_length = (_Traits::template _Storage_size<_Args> +...);

		// The actual storage representation: _Num_args offsets into _Storage, followed immediately by the untyped
		// _Storage which holds copies of the object representations of arguments (with no regard for alignment).
		// These must be allocated consecutively, since basic_format_args thinks it can store a pointer to
		// _Index_array and use arithmetic to access the bytes of _Storage.
		_Format_arg_index _Index_array[_Num_args];
		unsigned char _Storage[_Storage_length];

#pragma warning(push)
#pragma warning(disable : 6386) // Buffer overrun while writing to '%s' ...
		template <class T>
		void _Store_impl(
			const size_t _Arg_index, const _Basic_format_arg_type _Arg_type, const type_identity_t<T>& _Val) noexcept {

			const auto _Store_index = _Index_array[_Arg_index]._Index;

			memcpy(_Storage + _Store_index, std::addressof(_Val), sizeof(T));
			_Index_array[_Arg_index]._Type(_Arg_type);
			if (_Arg_index + 1 < _Num_args) {
				// Set the starting index of the next arg, as that is dynamic, must be called with increasing index
				_Index_array[_Arg_index + 1] = _Format_arg_index{ _Store_index + sizeof(T) };
			}
		}
#pragma warning(pop)

		template <class T>
		void _Store(const size_t _Arg_index, T&& _Val) noexcept {
			using _Erased_type = typename _Traits::template _Storage_type<T>;

			_Basic_format_arg_type _Arg_type;
			if constexpr (is_same_v<_Erased_type, bool>) {
				_Arg_type = _Basic_format_arg_type::_Bool_type;
			}
			else if constexpr (is_same_v<_Erased_type, _CharType>) {
				_Arg_type = _Basic_format_arg_type::_Char_type;
			}
			else if constexpr (is_same_v<_Erased_type, int>) {
				_Arg_type = _Basic_format_arg_type::_Int_type;
			}
			else if constexpr (is_same_v<_Erased_type, unsigned int>) {
				_Arg_type = _Basic_format_arg_type::_UInt_type;
			}
			else if constexpr (is_same_v<_Erased_type, long long>) {
				_Arg_type = _Basic_format_arg_type::_Long_long_type;
			}
			else if constexpr (is_same_v<_Erased_type, unsigned long long>) {
				_Arg_type = _Basic_format_arg_type::_ULong_long_type;
			}
			else if constexpr (is_same_v<_Erased_type, float>) {
				_Arg_type = _Basic_format_arg_type::_Float_type;
			}
			else if constexpr (is_same_v<_Erased_type, double>) {
				_Arg_type = _Basic_format_arg_type::_Double_type;
			}
			else if constexpr (is_same_v<_Erased_type, long double>) {
				_Arg_type = _Basic_format_arg_type::_Long_double_type;
			}
			else if constexpr (is_same_v<_Erased_type, const void*>) {
				_Arg_type = _Basic_format_arg_type::_Pointer_type;
			}
			else if constexpr (is_same_v<_Erased_type, const _CharType*>) {
				_Arg_type = _Basic_format_arg_type::_CString_type;
			}
			else if constexpr (is_same_v<_Erased_type, basic_string_view<_CharType>>) {
				_Arg_type = _Basic_format_arg_type::_String_type;
			}
			else {
				_STL_INTERNAL_STATIC_ASSERT(is_same_v<_Erased_type, typename basic_format_arg<_Context>::handle>);
				_Arg_type = _Basic_format_arg_type::_Custom_type;
			}

			// Workaround towards N4928 [format.arg]/9 and /10 in C++20
			if constexpr (is_same_v<_Erased_type, basic_string_view<_CharType>>) {
				_Store_impl<_Erased_type>(_Arg_index, _Arg_type, _Erased_type{ _Val.data(), _Val.size() });
			}
			else
			{
				_Store_impl<_Erased_type>(_Arg_index, _Arg_type, static_cast<_Erased_type>(_Val));
			}
		}

	public:
		_Format_arg_store(_Args&... _Vals) noexcept {
			_Index_array[0] = {};
			size_t _Arg_index = 0;
			(_Store(_Arg_index++, _Vals), ...);
		}
	};

	template <class _Context>
	class _Format_arg_store<_Context> {};

	template <class _Context>
		class basic_format_args {
		public:
			basic_format_args() noexcept = default;

			basic_format_args(const _Format_arg_store<_Context>&) noexcept {}

			template <class... _Args>
			basic_format_args(const _Format_arg_store<_Context, _Args...>& _Store) noexcept
				: _Num_args(sizeof...(_Args)), _Index_array(_Store._Index_array) {}

			[[nodiscard]] basic_format_arg<_Context> get(const size_t _Index) const noexcept {
				if (_Index >= _Num_args) {
					return basic_format_arg<_Context>{};
				}

				using _CharType = typename _Context::char_type;
				// The explanatory comment in _Format_arg_store explains how the following works.
				const auto _Packed_index = _Index_array[_Index];
				const auto _Arg_storage =
					reinterpret_cast<const unsigned char*>(_Index_array + _Num_args) + _Packed_index._Index;

				switch (_Packed_index._Type()) {
				case _Basic_format_arg_type::_None:
				default:
					_STL_ASSERT(false, "Invalid basic_format_arg type");
					return basic_format_arg<_Context>{};
				case _Basic_format_arg_type::_Int_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<int>(_Arg_storage)};
				case _Basic_format_arg_type::_UInt_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<unsigned int>(_Arg_storage)};
				case _Basic_format_arg_type::_Long_long_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<long long>(_Arg_storage)};
				case _Basic_format_arg_type::_ULong_long_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<unsigned long long>(_Arg_storage)};
				case _Basic_format_arg_type::_Bool_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<bool>(_Arg_storage)};
				case _Basic_format_arg_type::_Char_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<_CharType>(_Arg_storage)};
				case _Basic_format_arg_type::_Float_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<float>(_Arg_storage)};
				case _Basic_format_arg_type::_Double_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<double>(_Arg_storage)};
				case _Basic_format_arg_type::_Long_double_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<long double>(_Arg_storage)};
				case _Basic_format_arg_type::_Pointer_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<const void*>(_Arg_storage)};
				case _Basic_format_arg_type::_CString_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<const _CharType*>(_Arg_storage)};
				case _Basic_format_arg_type::_String_type:
					return basic_format_arg<_Context>{_Get_value_from_memory<basic_string_view<_CharType>>(_Arg_storage)};
				case _Basic_format_arg_type::_Custom_type:
					return basic_format_arg<_Context>{
						_Get_value_from_memory<typename basic_format_arg<_Context>::handle>(_Arg_storage)};
				}
			}

			[[nodiscard]] size_t _Estimate_required_capacity() const noexcept {
				using _CharT = typename _Context::char_type;
				size_t _Result = 0;
				const auto _Visitor = [&_Result]<class _ArgTy>(const _ArgTy arg) noexcept {
					if constexpr (is_same_v<_ArgTy, basic_string_view<_CharT>>) {
						_Result += arg.size();
					}
					else if constexpr (is_same_v<_ArgTy, const _CharT*>) {
						_Result += 32; // estimate for length of null-terminated strings
					}
					else {
						_Result += 8; // estimate for length of all other arguments
					}
				};
				for (size_t _Idx = 0; _Idx < _Num_args; ++_Idx) {
					visit_format_arg(_Visitor, get(_Idx));
				}
				return _Result;
			}

		private:
			template <class T>
			[[nodiscard]] static auto _Get_value_from_memory(const unsigned char* const _Val) noexcept {
				auto& _Temp = *reinterpret_cast<const unsigned char(*)[sizeof(T)]>(_Val);
				return bit_cast<T>(_Temp);
			}

			size_t _Num_args = 0;
			const _Format_arg_index* _Index_array = nullptr;
	};

	template <class _Context, class... _Args>
	basic_format_args(_Format_arg_store<_Context, _Args...>) -> basic_format_args<_Context>;

	template <integral _Integral>
	[[nodiscard]] constexpr string_view _Get_integral_prefix(const char _Type, const _Integral _Value) noexcept {
		switch (_Type) {
		case 'b':
			return "0b"sv;
		case 'B':
			return "0B"sv;
		case 'x':
			return "0x"sv;
		case 'X':
			return "0X"sv;
		case 'o':
			if (_Value != _Integral{ 0 }) {
				return "0"sv;
			}
			return {};
		default:
			return {};
		}
	}

	template <class _OutputIt>
	[[nodiscard]] _OutputIt _Write_sign(_OutputIt _Out, const _Fmt_sign _Sgn, const bool _Is_negative) {
		if (_Is_negative) {
			*_Out++ = '-';
		}
		else {
			switch (_Sgn) {
			case _Fmt_sign::_Plus:
				*_Out++ = '+';
				break;
			case _Fmt_sign::_Space:
				*_Out++ = ' ';
				break;
			case _Fmt_sign::_None:
			case _Fmt_sign::_Minus:
				break;
			}
		}
		return _Out;
	}

}