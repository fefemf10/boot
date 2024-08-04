export module sl.string_view;
import sl.type;
import sl.typetraits;
import sl.chartraits;
import sl.concepts;
import sl.numeric_limits;
import sl.algorithm;
import sl.compare;

export namespace std
{
	template <class T, class int_type, class Traits = std::char_traits<T, int_type>>
	class basic_string_view
	{
	public:
		using traits_type = Traits;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using const_iterator = const T*;
		using iterator = const_iterator;
		//using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		//using reverse_iterator = const_reverse_iterator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		static constexpr auto npos{ static_cast<size_type>(-1) };

		constexpr basic_string_view() noexcept : m_data(), m_size(0) {}

		constexpr basic_string_view(const basic_string_view& strv) noexcept : m_data(strv.m_data), m_size(strv.m_size)
		{

		}
		constexpr basic_string_view& operator=(const basic_string_view& strv) noexcept
		{
			m_data = strv.m_data;
			m_size = strv.m_size;
			return *this;
		}

		constexpr basic_string_view(const_pointer str) : m_data(str), m_size(traits_type::length(str)) {}

		basic_string_view(nullptr_t) = delete;

		constexpr basic_string_view(const_pointer str, size_type size) : m_data(str), m_size(size) {}

		constexpr const_iterator begin() const noexcept { return const_iterator(m_data); }
		constexpr const_iterator end() const noexcept { return const_iterator(m_data + m_size); }
		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr const_iterator cend() const noexcept { return end(); }

		constexpr size_type size() const noexcept { return m_size; }
		constexpr size_type length() const noexcept { return size(); }
		constexpr bool empty() const noexcept { return m_size == 0; }
		constexpr const_pointer data() const noexcept { return m_data; }
		constexpr size_type max_size() const noexcept { return numeric_limits<size_type>::max(); }

		//const_reverse_iterator rbegin() const noexcept {
		//	return const_reverse_iterator(end());
		//}
		//const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		//const_reverse_iterator rend() const noexcept {
		//	return const_reverse_iterator(begin());
		//}
		//const_reverse_iterator crend() const noexcept { return rend(); }

		constexpr const_reference operator[](size_type pos) const { return m_data[pos]; }

		constexpr const_reference at(size_type pos) const
		{
			if constexpr (pos > m_size)
				static_assert(pos > m_size, "Out of range");

			return this->operator[](pos);
		}
		constexpr const_reference front() const { return m_data[0]; }
		constexpr const_reference back() const { return m_data[m_size - 1]; }

		constexpr void remove_prefix(size_type n) { m_data = m_data + n; m_size -= n; }
		constexpr void remove_suffix(size_type n) { m_size -= n; }

		constexpr void swap(basic_string_view& s) noexcept
		{
			basic_string_view other = *this;
			*this = s;
			s = other;
		}

		constexpr size_type copy(pointer dest, size_type count, size_type pos = 0) const
		{
			if constexpr (pos > m_size)
				static_assert(pos > m_size, "Out of range");

			const size_type rcount = std::min(count, m_size - pos);
			traits_type::copy(dest, m_data + pos, rcount);
			return rcount;
		}
		[[nodiscard]] constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const
		{
			if constexpr (pos > m_size)
				static_assert(pos > m_size, "Out of range");

			const size_type rcount = std::min(count, m_size - pos);
			if (rcount > 0) {
				return basic_string_view(m_data + pos, rcount);
			}
			return basic_string_view();
		}

		constexpr bool _Equal(const basic_string_view rhs) const noexcept
		{
			return _Traits_equal<traits_type>(m_data, m_size, rhs.m_data, rhs.m_size);
		}

		[[nodiscard]] constexpr int compare(basic_string_view s) const noexcept
		{
			const size_t rlen = std::min(m_size, s.m_size);
			const int comparison = traits_type::compare(m_data, s.m_data, rlen);
			if (comparison != 0) return comparison;
			if (m_size == s.m_size) return 0;
			return m_size < s.m_size ? -1 : 1;
		}
		[[nodiscard]] constexpr int compare(size_type pos1, size_type count1, basic_string_view s) const { return substr(pos1, count1).compare(s); }
		[[nodiscard]] constexpr int compare(size_type pos1, size_type count1, basic_string_view s, size_type pos2, size_type count2) const { return substr(pos1, count1).compare(s.substr(pos2, count2)); }
		[[nodiscard]] constexpr int compare(const_pointer s) const { return compare(basic_string_view(s)); }
		[[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const_pointer s) const { return substr(pos1, count1).compare(basic_string_view(s)); }
		[[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const { return substr(pos1, count1).compare(basic_string_view(s, count2)); }
		
		[[nodiscard]] constexpr bool starts_with(basic_string_view s) const noexcept { return m_size >= s.m_size && substr(0, s.m_size) == s; }
		[[nodiscard]] constexpr bool starts_with(value_type c) const noexcept { return !empty() && traits_type::eq(front(), c); }
		[[nodiscard]] constexpr bool starts_with(const_pointer s) const { return starts_with(basic_string_view<T, traits_type>(s)); }

		[[nodiscard]] constexpr bool ends_with(basic_string_view s) const noexcept { return m_size >= s.m_size && substr(m_size - s.m_size, npos) == s; }
		[[nodiscard]] constexpr bool ends_with(value_type c) const noexcept { return !empty() && traits_type::eq(back(), c); }
		[[nodiscard]] constexpr bool ends_with(const_pointer s) const { return ends_with(basic_string_view<T, traits_type>(s)); }

		[[nodiscard]] constexpr bool contains(const basic_string_view right) const noexcept { return find(right) != npos; }
		[[nodiscard]] constexpr bool contains(const T right) const noexcept { return find(right) != npos; }
		[[nodiscard]] constexpr bool contains(const T* const right) const noexcept { return find(right) != npos; }

		[[nodiscard]] constexpr size_type find(basic_string_view s, size_type pos = 0) const noexcept {
			if (empty() && s.empty() && pos == 0) {
				return 0;
			}
			if (pos > m_size || s.m_size > (m_size - pos)) {
				return npos;
			}
			while (pos + s.m_size <= m_size) {
				if (traits_type::compare(m_data + pos, s.m_data, s.m_size) == 0) {
					return pos;
				}

				pos++;
			}

			return npos;
		}
		[[nodiscard]] constexpr size_type find(value_type c, size_type pos = 0) const noexcept { return find(basic_string_view(&c, 1), pos); }
		[[nodiscard]] constexpr size_type find(const_pointer s, size_type pos, size_type n) const { return find(basic_string_view(s, n), pos); }
		[[nodiscard]] constexpr size_type find(const_pointer s, size_type pos = 0) const { return find(basic_string_view(s), pos); }
		[[nodiscard]] constexpr size_type rfind(basic_string_view s, size_type pos = npos) const noexcept
		{
			if (s.empty()) {
				return std::min(pos, m_size);
			}
			if (s.m_size > m_size) {
				return npos;
			}
			pos = std::min(pos, m_size - s.m_size);
			while (pos != npos) {
				if (traits_type::compare(m_data + pos, s.m_data, s.m_size) == 0) {
					return pos;
				}

				pos--;
			}

			return npos;
		}
		[[nodiscard]] constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept { return rfind(basic_string_view(&c, 1), pos); }
		[[nodiscard]] constexpr size_type rfind(const_pointer s, size_type pos, size_type n) const { return rfind(basic_string_view(s, n), pos); }
		[[nodiscard]] constexpr size_type rfind(const_pointer s, size_type pos = npos) const { return rfind(basic_string_view(s), pos); }
		[[nodiscard]] constexpr size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept
		{
			while (pos < m_size) {
				if (traits_type::find(s.m_data, s.m_size, m_data[pos]) != nullptr) {
					return pos;
				}

				pos++;
			}

			return npos;
		}
		[[nodiscard]] constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept { return find_first_of(basic_string_view(&c, 1), pos); }
		[[nodiscard]] constexpr size_type find_first_of(const_pointer s, size_type pos, size_type n) const { return find_first_of(basic_string_view(s, n), pos); }
		[[nodiscard]] constexpr size_type find_first_of(const_pointer s, size_type pos = 0) const { return find_first_of(basic_string_view(s), pos); }
		[[nodiscard]] constexpr size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept
		{
			if (empty()) {
				return npos;
			}

			pos = std::min(pos, m_size - 1);
			while (pos != npos) {
				if (traits_type::find(s.m_data, s.m_size, m_data[pos]) != nullptr) {
					return pos;
				}

				pos--;
			}

			return npos;
		}
		constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept { return find_last_of(basic_string_view(&c, 1), pos);	}
		constexpr size_type find_last_of(const_pointer s, size_type pos, size_type n) const { return find_last_of(basic_string_view(s, n), pos);	}
		constexpr size_type find_last_of(const_pointer s, size_type pos = npos) const { return find_last_of(basic_string_view(s), pos);	}
		constexpr size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept
		{
			while (pos < m_size)
			{
				if (traits_type::find(s.m_data, s.m_size, m_data[pos]) == nullptr)
				{
					return pos;
				}
				pos++;
			}
			return npos;
		}
		constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept { return find_first_not_of(basic_string_view(&c, 1), pos); }
		constexpr size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const { return find_first_not_of(basic_string_view(s, n), pos); }
		constexpr size_type find_first_not_of(const_pointer s, size_type pos = 0) const { return find_first_not_of(basic_string_view(s), pos); }
		constexpr size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept
		{
			if (empty())
			{
				return npos;
			}

			pos = std::min(pos, m_size - 1);
			while (pos != npos) {
				if (traits_type::find(s.m_data, s.m_size, m_data[pos]) == nullptr) {
					return pos;
				}

				pos--;
			}

			return npos;
		}
		constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept { return find_last_not_of(basic_string_view(&c, 1), pos); }
		constexpr size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const { return find_last_not_of(basic_string_view(s, n), pos); }
		constexpr size_type find_last_not_of(const_pointer s, size_type pos = npos) const { return find_last_not_of(basic_string_view(s), pos); }

	private:
		const_pointer m_data;
		size_type m_size;
	};

	template <class T, class Traits>
	[[nodiscard]] constexpr bool operator==(const basic_string_view<T, Traits> lhs, const basic_string_view<T, Traits> rhs) noexcept
	{
		return lhs._Equal(rhs);
	}

	template <class T, class = void>
	struct _Get_comparison_category {
		using type = weak_ordering;
	};

	template <class T>
	struct _Get_comparison_category<T, void_t<typename T::comparison_category>> {
		using type = typename T::comparison_category;

		static_assert(_Is_any_of_v<type, partial_ordering, weak_ordering, strong_ordering>,
			"N4950 [string.view.comparison]/4: Mandates: R denotes a comparison category type.");
	};

	template <class T>
	using _Get_comparison_category_t = typename _Get_comparison_category<T>::type;

	template <class T, class Traits>
	[[nodiscard]] constexpr _Get_comparison_category_t<Traits> operator<=>(const basic_string_view<T, Traits> left, const basic_string_view<T, Traits> right) noexcept
	{
		return static_cast<_Get_comparison_category_t<Traits>>(left.compare(right) <=> 0);
	}

	template <class T, class Traits, int = 2>
	[[nodiscard]] constexpr _Get_comparison_category_t<Traits> operator<=>(const basic_string_view<T, Traits> left, const _Identity_t<basic_string_view<T, Traits>> right) noexcept
	{
		return static_cast<_Get_comparison_category_t<Traits>>(left.compare(right) <=> 0);
	}
	export using string_view = std::basic_string_view<char, unsigned char>;
	export using u8string_view = std::basic_string_view<char8_t, unsigned int>;
	export using u16string_view = std::basic_string_view<char16_t, unsigned short>;
	export using u32string_view = std::basic_string_view<char32_t, unsigned int>;
	inline namespace literals {
		inline namespace string_view_literals {
			export [[nodiscard]] constexpr inline string_view operator""sv(const char* str, std::size_t len) noexcept { return string_view(str, len); }
			export [[nodiscard]] constexpr inline u8string_view operator""sv(const char8_t* str, std::size_t len) noexcept { return u8string_view(str, len); }
			export [[nodiscard]] constexpr inline u16string_view operator""sv(const char16_t* str, std::size_t len) noexcept { return u16string_view(str, len); }
			export [[nodiscard]] constexpr inline u32string_view operator""sv(const char32_t* str, std::size_t len) noexcept { return u32string_view(str, len); }
		}
	}
}
