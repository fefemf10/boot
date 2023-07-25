export module sl.atomic;
import sl.typetraits;
import sl.concepts;
import sl.memory;
import sl.type;
import intrinsic;

inline constexpr unsigned long long _Atomic_wait_no_deadline = 0xFFFF'FFFF'FFFF'FFFF;
inline constexpr unsigned long _Atomic_wait_no_timeout = 0xFFFF'FFFF; // Pass as partial timeout
export namespace std
{
	enum class memory_order : int {
		relaxed,
		consume,
		acquire,
		release,
		acq_rel,
		seq_cst,

		// LWG-3268
		memory_order_relaxed = relaxed,
		memory_order_consume = consume,
		memory_order_acquire = acquire,
		memory_order_release = release,
		memory_order_acq_rel = acq_rel,
		memory_order_seq_cst = seq_cst
	};
	inline constexpr memory_order memory_order_relaxed = memory_order::relaxed;
	inline constexpr memory_order memory_order_consume = memory_order::consume;
	inline constexpr memory_order memory_order_acquire = memory_order::acquire;
	inline constexpr memory_order memory_order_release = memory_order::release;
	inline constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
	inline constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;

	using _Atomic_counter_t = unsigned long;

	template <std::integral int_type, class T>
	[[nodiscard]] volatile int_type* _Atomic_address_as(T& _Source) noexcept
	{
		return &reinterpret_cast<volatile int_type&>(_Source);
	}

	template <std::integral int_type, class T>
	[[nodiscard]] const volatile int_type* _Atomic_address_as(const T& _Source) noexcept
	{
		return &reinterpret_cast<const volatile int_type&>(_Source);
	}
}
enum
{
	_Atomic_memory_order_relaxed,
	_Atomic_memory_order_consume,
	_Atomic_memory_order_acquire,
	_Atomic_memory_order_release,
	_Atomic_memory_order_acq_rel,
	_Atomic_memory_order_seq_cst,
};

#define ATOMIC_BOOL_LOCK_FREE 2
#define ATOMIC_CHAR_LOCK_FREE 2
#define ATOMIC_CHAR8_T_LOCK_FREE 2
#define ATOMIC_CHAR16_T_LOCK_FREE 2
#define ATOMIC_CHAR32_T_LOCK_FREE 2
#define ATOMIC_WCHAR_T_LOCK_FREE  2
#define ATOMIC_SHORT_LOCK_FREE    2
#define ATOMIC_INT_LOCK_FREE      2
#define ATOMIC_LONG_LOCK_FREE     2
#define ATOMIC_LLONG_LOCK_FREE    2
#define ATOMIC_POINTER_LOCK_FREE  2

// The following code is SHARED with vcruntime and any updates
// should be mirrored. Also: if any macros are added they should be
// #undefed in vcruntime as well

extern "C" [[nodiscard]] char __stdcall __std_atomic_has_cmpxchg16b() noexcept;

extern "C" void _Check_memory_order(const unsigned int _Order) {
	if (_Order > _Atomic_memory_order_seq_cst) {
		;
	}
}
// note: these macros are _not_ always safe to use with a trailing semicolon,
// we avoid wrapping them in do {} while (0) because MSVC generates code for such loops
// in debug mode.

#define _ATOMIC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...) \
	_Check_memory_order(_Order);                                   \
	_Result = _Intrinsic(__VA_ARGS__)


#define _ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(_Order_var) \
	switch (_Order_var) {                               \
	case _Atomic_memory_order_relaxed:                  \
		break;                                          \
	case _Atomic_memory_order_consume:                  \
	case _Atomic_memory_order_acquire:                  \
	case _Atomic_memory_order_seq_cst:                  \
		_ReadWriteBarrier();                  \
		break;                                          \
	case _Atomic_memory_order_release:                  \
	case _Atomic_memory_order_acq_rel:                  \
	default:                                            \
		;                          \
		break;                                          \
	}

#define __STORE_RELEASE(_Width, _Ptr, _Desired) \
	_ReadWriteBarrier();              \
	__iso_volatile_store##_Width((_Ptr), (_Desired));


#define _ATOMIC_STORE_PREFIX(_Width, _Ptr, _Desired)      \
	case _Atomic_memory_order_relaxed:                    \
		__iso_volatile_store##_Width((_Ptr), (_Desired)); \
		return;                                           \
	case _Atomic_memory_order_release:                    \
		__STORE_RELEASE(_Width, _Ptr, _Desired)           \
		return;                                           \
	default:                                              \
	case _Atomic_memory_order_consume:                    \
	case _Atomic_memory_order_acquire:                    \
	case _Atomic_memory_order_acq_rel:                    \
		;                            \
		[[fallthrough]];

#define _ATOMIC_STORE_SEQ_CST_ARM(_Width, _Ptr, _Desired) \
	_Memory_barrier();                                    \
	__iso_volatile_store##_Width((_Ptr), (_Desired));     \
	_Memory_barrier();

#if _STD_ATOMIC_USE_ARM64_LDAR_STLR == 1
#define _ATOMIC_STORE_SEQ_CST_ARM64(_Width, _Ptr, _Desired)                                                       \
	_ReadWriteBarrier();                                                                                          \
	__stlr##_Width(                                                                                               \
		reinterpret_cast<volatile unsigned __int##_Width*>(_Ptr), static_cast<unsigned __int##_Width>(_Desired)); \
	_Memory_barrier();
#else
#define _ATOMIC_STORE_SEQ_CST_ARM64 _ATOMIC_STORE_SEQ_CST_ARM
#endif

#define _ATOMIC_STORE_SEQ_CST_X86_X64(_Width, _Ptr, _Desired) (void) _InterlockedExchange##_Width((_Ptr), (_Desired));
#define _ATOMIC_STORE_32_SEQ_CST_X86_X64(_Ptr, _Desired) \
	(void) _InterlockedExchange(reinterpret_cast<volatile long*>(_Ptr), static_cast<long>(_Desired));

#define _ATOMIC_STORE_64_SEQ_CST_IX86(_Ptr, _Desired) \
	_ReadWriteBarrier();                              \
	__iso_volatile_store64((_Ptr), (_Desired));       \
	_Atomic_thread_fence(_Atomic_memory_order_seq_cst);


#define _ATOMIC_STORE_SEQ_CST(_Width, _Ptr, _Desired) _ATOMIC_STORE_SEQ_CST_X86_X64(_Width, (_Ptr), (_Desired))
#define _ATOMIC_STORE_32_SEQ_CST(_Ptr, _Desired)      _ATOMIC_STORE_32_SEQ_CST_X86_X64((_Ptr), (_Desired))
#define _ATOMIC_STORE_64_SEQ_CST(_Ptr, _Desired) _ATOMIC_STORE_SEQ_CST_X86_X64(64, (_Ptr), (_Desired))

#pragma warning(push)
#pragma warning(disable : 6001) // "Using uninitialized memory '_Guard'"
#pragma warning(disable : 28113) // "Accessing a local variable _Guard via an Interlocked function: This is an unusual
								 // usage which could be reconsidered."


extern "C" inline void _Atomic_thread_fence(const unsigned int _Order) noexcept {
	if (_Order == _Atomic_memory_order_relaxed) {
		return;
	}
	_ReadWriteBarrier();
	if (_Order == _Atomic_memory_order_seq_cst) {
		volatile long _Guard; // Not initialized to avoid an unnecessary operation; the value does not matter

		// _mm_mfence could have been used, but it is not supported on older x86 CPUs and is slower on some recent CPUs.
		// The memory fence provided by interlocked operations has some exceptions, but this is fine:
		// std::atomic_thread_fence works with respect to other atomics only; it may not be a full fence for all ops.
		(void)_InterlockedIncrement(&_Guard);
		_ReadWriteBarrier();
	}
}
#pragma warning(pop)

using _Smtx_t = void*;
void __cdecl _Smtx_lock_exclusive(_Smtx_t*);
void __cdecl _Smtx_lock_shared(_Smtx_t*);
int __cdecl _Smtx_try_lock_exclusive(_Smtx_t*);
int __cdecl _Smtx_try_lock_shared(_Smtx_t*);
void __cdecl _Smtx_unlock_exclusive(_Smtx_t*);
void __cdecl _Smtx_unlock_shared(_Smtx_t*);
extern "C" _Smtx_t * __stdcall __std_atomic_get_mutex(const void* _Key) noexcept;

#define _CMPXCHG_MASK_OUT_PADDING_BITS 0
export namespace std
{

	template <class T>
	struct _Storage_for {
		// uninitialized space to store a T
		alignas(T) unsigned char _Storage[sizeof(T)];

		_Storage_for() = default;
		_Storage_for(const _Storage_for&) = delete;
		_Storage_for& operator=(const _Storage_for&) = delete;


		[[nodiscard]] T& _Ref() noexcept {
			return reinterpret_cast<T&>(_Storage);
		}

		[[nodiscard]] T* _Ptr() noexcept {
			return reinterpret_cast<T*>(&_Storage);
		}
	};

	extern "C" inline void atomic_thread_fence(const memory_order _Order) noexcept {
		::_Atomic_thread_fence(static_cast<unsigned int>(_Order));
	}

	extern "C" inline void atomic_signal_fence(const memory_order _Order) noexcept {
		if (_Order != memory_order_relaxed) {
			_ReadWriteBarrier();
		}
	}

	template <class T>
	T kill_dependency(T _Arg) noexcept { // "magic" template that kills dependency ordering when called
		return _Arg;
	}

	inline void _Check_store_memory_order(const memory_order _Order) noexcept {
		switch (_Order) {
		case memory_order_relaxed:
		case memory_order_release:
		case memory_order_seq_cst:
			// nothing to do
			break;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		default:
			;
			break;
		}
	}

	inline void _Check_load_memory_order(const memory_order _Order) noexcept {
		switch (_Order) {
		case memory_order_relaxed:
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_seq_cst:
			// nothing to do
			break;
		case memory_order_release:
		case memory_order_acq_rel:
		default:
			;
			break;
		}
	}

	[[nodiscard]] inline memory_order _Combine_cas_memory_orders(
		const memory_order _Success, const memory_order _Failure) noexcept {
		// Finds upper bound of a compare/exchange memory order
		// pair, according to the following partial order:
		//     seq_cst
		//        |
		//     acq_rel
		//     /     \
		// acquire  release
		//    |       |
		// consume    |
		//     \     /
		//     relaxed
		static constexpr memory_order _Combined_memory_orders[6][6] = {// combined upper bounds
			{memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel,
				memory_order_seq_cst},
			{memory_order_consume, memory_order_consume, memory_order_acquire, memory_order_acq_rel, memory_order_acq_rel,
				memory_order_seq_cst},
			{memory_order_acquire, memory_order_acquire, memory_order_acquire, memory_order_acq_rel, memory_order_acq_rel,
				memory_order_seq_cst},
			{memory_order_release, memory_order_acq_rel, memory_order_acq_rel, memory_order_release, memory_order_acq_rel,
				memory_order_seq_cst},
			{memory_order_acq_rel, memory_order_acq_rel, memory_order_acq_rel, memory_order_acq_rel, memory_order_acq_rel,
				memory_order_seq_cst},
			{memory_order_seq_cst, memory_order_seq_cst, memory_order_seq_cst, memory_order_seq_cst, memory_order_seq_cst,
				memory_order_seq_cst} };

		_Check_memory_order(static_cast<unsigned int>(_Success));
		_Check_load_memory_order(_Failure);
		return _Combined_memory_orders[static_cast<int>(_Success)][static_cast<int>(_Failure)];
	}

	template <class _Integral, class T>
	[[nodiscard]] _Integral _Atomic_reinterpret_as(const T& _Source) noexcept {
		// interprets _Source as the supplied integral type
		static_assert(is_integral_v<_Integral>, "Tried to reinterpret memory as non-integral");
		if constexpr (is_integral_v<T> && sizeof(_Integral) == sizeof(T)) {
			return static_cast<_Integral>(_Source);
		}
		else if constexpr (is_pointer_v<T> && sizeof(_Integral) == sizeof(T)) {
			return reinterpret_cast<_Integral>(_Source);
		}
		else {
			_Integral _Result{}; // zero padding bits
			memcpy(&_Result, addressof(_Source), sizeof(_Source));
			return _Result;
		}
	}

	template <class T>
	struct _Atomic_padded {
		alignas(sizeof(T)) mutable T _Value; // align to sizeof(T); x86 stack aligns 8-byte objects on 4-byte boundaries
	};

	template <class T>
	struct _Atomic_storage_types {
		using _TStorage = _Atomic_padded<T>;
		using _Spinlock = long;
	};

	template <class T>
	struct _Atomic_storage_types<T&> {
		using _TStorage = T&;
		using _Spinlock = _Smtx_t*; // POINTER TO mutex
	};

	template <class T, size_t = sizeof(remove_reference_t<T>)>
	struct _Atomic_storage;

	template <class T, class _Value_type>
	void _Atomic_wait_direct(
		const _Atomic_storage<T>* const _This, _Value_type _Expected_bytes, const memory_order _Order) noexcept {
		const auto _Storage_ptr = addressof(_This->_Storage);
		for (;;) {
			const _Value_type _Observed_bytes = _Atomic_reinterpret_as<_Value_type>(_This->load(_Order));
			if (_Expected_bytes != _Observed_bytes) {
				return;
			}

			//__std_atomic_wait_direct(_Storage_ptr, &_Expected_bytes, sizeof(_Value_type), _Atomic_wait_no_timeout);
		}
	}

	inline void _Atomic_lock_acquire(long& _Spinlock) noexcept {
		// Algorithm from Intel(R) 64 and IA-32 Architectures Optimization Reference Manual, May 2020
		// Example 2-4. Contended Locks with Increasing Back-off Example - Improved Version, page 2-22
		// The code in mentioned manual is covered by the 0BSD license.
		int _Current_backoff = 1;
		constexpr int _Max_backoff = 64;
		while (_InterlockedExchange(&_Spinlock, 1) != 0) {
			while (__iso_volatile_load32(&reinterpret_cast<int&>(_Spinlock)) != 0) {
				for (int _Count_down = _Current_backoff; _Count_down != 0; --_Count_down) {
					_mm_pause();
				}
				_Current_backoff = _Current_backoff < _Max_backoff ? _Current_backoff << 1 : _Max_backoff;
			}
		}
	}

	inline void _Atomic_lock_release(long& _Spinlock) noexcept {
		_InterlockedExchange(&_Spinlock, 0);
	}

	inline void _Atomic_lock_acquire(_Smtx_t* _Spinlock) noexcept {
		_Smtx_lock_exclusive(_Spinlock);
	}

	inline void _Atomic_lock_release(_Smtx_t* _Spinlock) noexcept {
		_Smtx_unlock_exclusive(_Spinlock);
	}

	template <class _Spinlock_t>
	class [[nodiscard]] _Atomic_lock_guard {
	public:
		explicit _Atomic_lock_guard(_Spinlock_t& _Spinlock_) noexcept : _Spinlock(_Spinlock_) {
			_Atomic_lock_acquire(_Spinlock);
		}

		~_Atomic_lock_guard() {
			_Atomic_lock_release(_Spinlock);
		}

		_Atomic_lock_guard(const _Atomic_lock_guard&) = delete;
		_Atomic_lock_guard& operator=(const _Atomic_lock_guard&) = delete;

	private:
		_Spinlock_t& _Spinlock;
	};

	template <class _Spinlock_t>
	bool __stdcall _Atomic_wait_compare_non_lock_free(
		const void* _Storage, void* _Comparand, size_t _Size, void* _Spinlock_raw) noexcept {
		_Spinlock_t& _Spinlock = *static_cast<_Spinlock_t*>(_Spinlock_raw);
		_Atomic_lock_acquire(_Spinlock);
		const auto _Cmp_result = memcmp(_Storage, _Comparand, _Size);
		_Atomic_lock_release(_Spinlock);
		return _Cmp_result == 0;
	}

	inline bool __stdcall _Atomic_wait_compare_16_bytes(const void* _Storage, void* _Comparand, size_t, void*) noexcept {
		const auto _Dest = static_cast<long long*>(const_cast<void*>(_Storage));
		const auto _Cmp = static_cast<const long long*>(_Comparand);
		alignas(16) long long _Tmp[2] = { _Cmp[0], _Cmp[1] };
		return _InterlockedCompareExchange128_np(_Dest, _Tmp[1], _Tmp[0], _Tmp) != 0;
	}

	template <class T, size_t /* = ... */>
	struct _Atomic_storage {
		// Provides operations common to all specializations of std::atomic, load, store, exchange, and CAS.
		// Locking version used when hardware has no atomic operations for sizeof(T).

		using _TVal = remove_reference_t<T>;
		using _Guard = _Atomic_lock_guard<typename _Atomic_storage_types<T>::_Spinlock>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T>, T, const _TVal> _Value) noexcept
			: _Storage(_Value) {
			// non-atomically initialize this atomic
		}

		void store(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// store with sequential consistency
			_Check_store_memory_order(_Order);
			_Guard _Lock{ _Spinlock };
			_Storage = _Value;
		}

		[[nodiscard]] _TVal load(const memory_order _Order = memory_order_seq_cst) const noexcept {
			// load with sequential consistency
			_Check_load_memory_order(_Order);
			_Guard _Lock{ _Spinlock };
			_TVal _Local(_Storage);
			return _Local;
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// exchange _Value with _Storage with sequential consistency
			_Check_memory_order(static_cast<unsigned int>(_Order));
			_Guard _Lock{ _Spinlock };
			_TVal _Result(_Storage);
			_Storage = _Value;
			return _Result;
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with sequential consistency, plain
			_Check_memory_order(static_cast<unsigned int>(_Order));
			const auto _Storage_ptr = addressof(_Storage);
			const auto _Expected_ptr = addressof(_Expected);
			bool _Result;
			_Guard _Lock{ _Spinlock };

			_Result = memcmp(_Storage_ptr, _Expected_ptr, sizeof(_TVal)) == 0;
			if (_Result) {
				memcpy(_Storage_ptr, addressof(_Desired), sizeof(_TVal));
			}
			else {
				memcpy(_Expected_ptr, _Storage_ptr, sizeof(_TVal));
			}

			return _Result;
		}

		void wait(_TVal _Expected, memory_order = memory_order_seq_cst) const noexcept {
			const auto _Storage_ptr = addressof(_Storage);
			const auto _Expected_ptr = addressof(_Expected);
			for (;;) {
				{
					_Guard _Lock{ _Spinlock };
					if (memcmp(_Storage_ptr, _Expected_ptr, sizeof(_TVal)) != 0) {
						{
							return;
						}
					}
				}

				__std_atomic_wait_indirect(_Storage_ptr, _Expected_ptr, sizeof(_TVal), &_Spinlock,
					&_Atomic_wait_compare_non_lock_free<decltype(_Spinlock)>, _Atomic_wait_no_timeout);
			}
		}

		void notify_one() noexcept {
			__std_atomic_notify_one_indirect(addressof(_Storage));
		}

		void notify_all() noexcept {
			__std_atomic_notify_all_indirect(addressof(_Storage));
		}

	protected:
		void _Init_spinlock_for_ref() noexcept {
			_Spinlock = __std_atomic_get_mutex(addressof(_Storage));
		}

	private:
		mutable typename _Atomic_storage_types<T>::_Spinlock _Spinlock{};

	public:
		T _Storage{};
	};

	template <class T>
	struct _Atomic_storage<T, 1> { // lock-free using 1-byte intrinsics

		using _TVal = remove_reference_t<T>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T>, T, const _TVal> _Value) noexcept
			: _Storage{ _Value } {
			// non-atomically initialize this atomic
		}

		void store(const _TVal _Value) noexcept { // store with sequential consistency
			const auto _Mem = _Atomic_address_as<char>(_Storage);
			const char _As_bytes = _Atomic_reinterpret_as<char>(_Value);
			_ATOMIC_STORE_SEQ_CST(8, _Mem, _As_bytes)
		}

		void store(const _TVal _Value, const memory_order _Order) noexcept { // store with given memory order
			const auto _Mem = _Atomic_address_as<char>(_Storage);
			const char _As_bytes = _Atomic_reinterpret_as<char>(_Value);
			switch (static_cast<unsigned int>(_Order)) {
				_ATOMIC_STORE_PREFIX(8, _Mem, _As_bytes)
			case _Atomic_memory_order_seq_cst:
				store(_Value);
				return;
			}
		}

		[[nodiscard]] _TVal load() const noexcept { // load with sequential consistency
			const auto _Mem = _Atomic_address_as<char>(_Storage);
			char _As_bytes = __iso_volatile_load8(_Mem);
			_ReadWriteBarrier();
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		[[nodiscard]] _TVal load(const memory_order _Order) const noexcept { // load with given memory order
			const auto _Mem = _Atomic_address_as<char>(_Storage);
			char _As_bytes;
			_As_bytes = __iso_volatile_load8(_Mem);
			_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(static_cast<unsigned int>(_Order))
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// exchange with given memory order
			char _As_bytes;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _As_bytes, _InterlockedExchange8,
				_Atomic_address_as<char>(_Storage), _Atomic_reinterpret_as<char>(_Value));
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with given memory order
			char _Expected_bytes = _Atomic_reinterpret_as<char>(_Expected); // read before atomic operation
			char _Prev_bytes;

			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Prev_bytes, _InterlockedCompareExchange8,
				_Atomic_address_as<char>(_Storage), _Atomic_reinterpret_as<char>(_Desired), _Expected_bytes);
			if (_Prev_bytes == _Expected_bytes) {
				return true;
			}

			reinterpret_cast<char&>(_Expected) = _Prev_bytes;
			return false;
		}

		void wait(const _TVal _Expected, const memory_order _Order = memory_order_seq_cst) const noexcept {
			_Atomic_wait_direct(this, _Atomic_reinterpret_as<char>(_Expected), _Order);
		}

		void notify_one() noexcept {
			//__std_atomic_notify_one_direct(addressof(_Storage));
		}

		void notify_all() noexcept {
			//__std_atomic_notify_all_direct(addressof(_Storage));
		}

		typename _Atomic_storage_types<T>::_TStorage _Storage;
	};

	template <class T>
	struct _Atomic_storage<T, 2> { // lock-free using 2-byte intrinsics

		using _TVal = remove_reference_t<T>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T>, T, const _TVal> _Value) noexcept
			: _Storage{ _Value } {
			// non-atomically initialize this atomic
		}

		void store(const _TVal _Value) noexcept { // store with sequential consistency
			const auto _Mem = _Atomic_address_as<short>(_Storage);
			const short _As_bytes = _Atomic_reinterpret_as<short>(_Value);
			_ATOMIC_STORE_SEQ_CST(16, _Mem, _As_bytes)
		}

		void store(const _TVal _Value, const memory_order _Order) noexcept { // store with given memory order
			const auto _Mem = _Atomic_address_as<short>(_Storage);
			const short _As_bytes = _Atomic_reinterpret_as<short>(_Value);
			switch (static_cast<unsigned int>(_Order)) {
				_ATOMIC_STORE_PREFIX(16, _Mem, _As_bytes)
			case _Atomic_memory_order_seq_cst:
				store(_Value);
				return;
			}
		}

		[[nodiscard]] _TVal load() const noexcept { // load with sequential consistency
			const auto _Mem = _Atomic_address_as<short>(_Storage);
			short _As_bytes = __iso_volatile_load16(_Mem);
			_ReadWriteBarrier();
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		[[nodiscard]] _TVal load(const memory_order _Order) const noexcept { // load with given memory order
			const auto _Mem = _Atomic_address_as<short>(_Storage);
			short _As_bytes;
#if _STD_ATOMIC_USE_ARM64_LDAR_STLR == 1
			_ATOMIC_LOAD_ARM64(_As_bytes, 16, _Mem, static_cast<unsigned int>(_Order))
#else
			_As_bytes = __iso_volatile_load16(_Mem);
			_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(static_cast<unsigned int>(_Order))
#endif
				return reinterpret_cast<_TVal&>(_As_bytes);
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// exchange with given memory order
			short _As_bytes;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _As_bytes, _InterlockedExchange16,
				_Atomic_address_as<short>(_Storage), _Atomic_reinterpret_as<short>(_Value));
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with given memory order
			short _Expected_bytes = _Atomic_reinterpret_as<short>(_Expected); // read before atomic operation
			short _Prev_bytes;
#if _CMPXCHG_MASK_OUT_PADDING_BITS
			if constexpr (_Might_have_non_value_bits<T>) {
				_Storage_for<_TVal> _Mask{ _Form_mask };
				const short _Mask_val = _Atomic_reinterpret_as<short>(_Mask._Ref());

				for (;;) {
					_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Prev_bytes, _InterlockedCompareExchange16,
						_Atomic_address_as<short>(_Storage), _Atomic_reinterpret_as<short>(_Desired), _Expected_bytes);
					if (_Prev_bytes == _Expected_bytes) {
						return true;
					}

					if ((_Prev_bytes ^ _Expected_bytes) & _Mask_val) {
						memcpy(addressof(_Expected), &_Prev_bytes, sizeof(_TVal));
						return false;
					}
					_Expected_bytes = (_Expected_bytes & _Mask_val) | (_Prev_bytes & ~_Mask_val);
				}
			}
#endif // _CMPXCHG_MASK_OUT_PADDING_BITS
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Prev_bytes, _InterlockedCompareExchange16,
				_Atomic_address_as<short>(_Storage), _Atomic_reinterpret_as<short>(_Desired), _Expected_bytes);
			if (_Prev_bytes == _Expected_bytes) {
				return true;
			}

			memcpy(addressof(_Expected), &_Prev_bytes, sizeof(T));
			return false;
		}

		void wait(const _TVal _Expected, const memory_order _Order = memory_order_seq_cst) const noexcept {
			_Atomic_wait_direct(this, _Atomic_reinterpret_as<short>(_Expected), _Order);
		}

		void notify_one() noexcept {
			//__std_atomic_notify_one_direct(addressof(_Storage));
		}

		void notify_all() noexcept {
			//__std_atomic_notify_all_direct(addressof(_Storage));
		}

		typename _Atomic_storage_types<T>::_TStorage _Storage;
	};

	template <class T>
	struct _Atomic_storage<T, 4> { // lock-free using 4-byte intrinsics

		using _TVal = remove_reference_t<T>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T>, T, const _TVal> _Value) noexcept
			: _Storage{ _Value } {
			// non-atomically initialize this atomic
		}

		void store(const _TVal _Value) noexcept { // store with sequential consistency
			const auto _Mem = _Atomic_address_as<int>(_Storage);
			const int _As_bytes = _Atomic_reinterpret_as<int>(_Value);
			_ATOMIC_STORE_32_SEQ_CST(_Mem, _As_bytes)
		}

		void store(const _TVal _Value, const memory_order _Order) noexcept { // store with given memory order
			const auto _Mem = _Atomic_address_as<int>(_Storage);
			const int _As_bytes = _Atomic_reinterpret_as<int>(_Value);
			switch (static_cast<unsigned int>(_Order)) {
				_ATOMIC_STORE_PREFIX(32, _Mem, _As_bytes)
			case _Atomic_memory_order_seq_cst:
				store(_Value);
				return;
			}
		}

		[[nodiscard]] _TVal load() const noexcept { // load with sequential consistency
			const auto _Mem = _Atomic_address_as<int>(_Storage);
			int _As_bytes = __iso_volatile_load32(_Mem);
			_ReadWriteBarrier();
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		[[nodiscard]] _TVal load(const memory_order _Order) const noexcept { // load with given memory order
			const auto _Mem = _Atomic_address_as<int>(_Storage);
			int _As_bytes;
			_As_bytes = __iso_volatile_load32(_Mem);
			_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(static_cast<unsigned int>(_Order))
				return reinterpret_cast<_TVal&>(_As_bytes);
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// exchange with given memory order
			long _As_bytes;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _As_bytes, _InterlockedExchange,
				_Atomic_address_as<long>(_Storage), _Atomic_reinterpret_as<long>(_Value));
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with given memory order
			long _Expected_bytes = _Atomic_reinterpret_as<long>(_Expected); // read before atomic operation
			long _Prev_bytes;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Prev_bytes, _InterlockedCompareExchange,
				_Atomic_address_as<long>(_Storage), _Atomic_reinterpret_as<long>(_Desired), _Expected_bytes);
			if (_Prev_bytes == _Expected_bytes) {
				return true;
			}

			memcpy(addressof(_Expected), &_Prev_bytes, sizeof(_TVal));
			return false;
		}

		void wait(const _TVal _Expected, const memory_order _Order = memory_order_seq_cst) const noexcept {
			_Atomic_wait_direct(this, _Atomic_reinterpret_as<long>(_Expected), _Order);
		}

		void notify_one() noexcept {
			//__std_atomic_notify_one_direct(addressof(_Storage));
		}

		void notify_all() noexcept {
			//__std_atomic_notify_all_direct(addressof(_Storage));
		}

		typename _Atomic_storage_types<T>::_TStorage _Storage;
	};

	template <class T>
	struct _Atomic_storage<T, 8> { // lock-free using 8-byte intrinsics

		using _TVal = remove_reference_t<T>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T>, T, const _TVal> _Value) noexcept
			: _Storage{ _Value } {
			// non-atomically initialize this atomic
		}

		void store(const _TVal _Value) noexcept { // store with sequential consistency
			const auto _Mem = _Atomic_address_as<long long>(_Storage);
			const long long _As_bytes = _Atomic_reinterpret_as<long long>(_Value);
			_ATOMIC_STORE_64_SEQ_CST(_Mem, _As_bytes);
		}

		void store(const _TVal _Value, const memory_order _Order) noexcept { // store with given memory order
			const auto _Mem = _Atomic_address_as<long long>(_Storage);
			const long long _As_bytes = _Atomic_reinterpret_as<long long>(_Value);
			switch (static_cast<unsigned int>(_Order)) {
				_ATOMIC_STORE_PREFIX(64, _Mem, _As_bytes)
			case _Atomic_memory_order_seq_cst:
				store(_Value);
				return;
			}
		}

		[[nodiscard]] _TVal load() const noexcept { // load with sequential consistency
			const auto _Mem = _Atomic_address_as<long long>(_Storage);
			long long _As_bytes = __iso_volatile_load64(_Mem);
			_ReadWriteBarrier();
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		[[nodiscard]] _TVal load(const memory_order _Order) const noexcept { // load with given memory order
			const auto _Mem = _Atomic_address_as<long long>(_Storage);
			long long _As_bytes;
			_As_bytes = __iso_volatile_load64(_Mem);
			_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(static_cast<unsigned int>(_Order))
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order = memory_order_seq_cst) noexcept {
			// exchange with given memory order
			long long _As_bytes;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _As_bytes, _InterlockedExchange64,
				_Atomic_address_as<long long>(_Storage), _Atomic_reinterpret_as<long long>(_Value));
			return reinterpret_cast<_TVal&>(_As_bytes);
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with given memory order
			long long _Expected_bytes = _Atomic_reinterpret_as<long long>(_Expected); // read before atomic operation
			long long _Prev_bytes;

			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Prev_bytes, _InterlockedCompareExchange64,
				_Atomic_address_as<long long>(_Storage), _Atomic_reinterpret_as<long long>(_Desired), _Expected_bytes);
			if (_Prev_bytes == _Expected_bytes) {
				return true;
			}

			memcpy(addressof(_Expected), &_Prev_bytes, sizeof(_TVal));
			return false;
		}

		void wait(const _TVal _Expected, const memory_order _Order = memory_order_seq_cst) const noexcept {
			_Atomic_wait_direct(this, _Atomic_reinterpret_as<long long>(_Expected), _Order);
		}

		void notify_one() noexcept {
			//__std_atomic_notify_one_direct(addressof(_Storage));
		}

		void notify_all() noexcept {
			//__std_atomic_notify_all_direct(addressof(_Storage));
		}

		typename _Atomic_storage_types<T>::_TStorage _Storage;
	};

	template <class T>
	struct _Atomic_storage<T&, 16> { // lock-free using 16-byte intrinsics
		// TRANSITION, ABI: replace 'T&' with 'T' in this specialization
		using _TVal = remove_reference_t<T&>;

		_Atomic_storage() = default;

		/* implicit */ constexpr _Atomic_storage(conditional_t<is_reference_v<T&>, T&, const _TVal> _Value) noexcept
			: _Storage{ _Value } {} // non-atomically initialize this atomic

		void store(const _TVal _Value) noexcept { // store with sequential consistency
			(void)exchange(_Value);
		}

		void store(const _TVal _Value, const memory_order _Order) noexcept { // store with given memory order
			_Check_store_memory_order(_Order);
			(void)exchange(_Value, _Order);
		}

		[[nodiscard]] _TVal load() const noexcept { // load with sequential consistency
			long long* const _Storage_ptr = const_cast<long long*>(_Atomic_address_as<const long long>(_Storage));
			_Int128 _Result{}; // atomic CAS 0 with 0
			(void)_InterlockedCompareExchange128_np(_Storage_ptr, 0, 0, &_Result._Low);
			return reinterpret_cast<_TVal&>(_Result);
		}

		[[nodiscard]] _TVal load(const memory_order _Order) const noexcept { // load with given memory order
			_Check_load_memory_order(_Order);
			return load();
		}

		_TVal exchange(const _TVal _Value) noexcept { // exchange with sequential consistency
			_TVal _Result{ _Value };
			while (!compare_exchange_strong(_Result, _Value)) { // keep trying
			}

			return _Result;
		}

		_TVal exchange(const _TVal _Value, const memory_order _Order) noexcept { // exchange with given memory order
			_TVal _Result{ _Value };
			while (!compare_exchange_strong(_Result, _Value, _Order)) { // keep trying
			}

			return _Result;
		}

		bool compare_exchange_strong(_TVal& _Expected, const _TVal _Desired,
			const memory_order _Order = memory_order_seq_cst) noexcept { // CAS with given memory order
			_Int128 _Desired_bytes{};
			memcpy(&_Desired_bytes, addressof(_Desired), sizeof(_TVal));
			_Int128 _Expected_temp{};
			memcpy(&_Expected_temp, addressof(_Expected), sizeof(_TVal));
			unsigned char _Result;
			(void)_Order;
			_Result = _InterlockedCompareExchange128_np(
				&reinterpret_cast<long long&>(_Storage), _Desired_bytes._High, _Desired_bytes._Low, &_Expected_temp._Low);
			if (_Result == 0) {
				memcpy(addressof(_Expected), &_Expected_temp, sizeof(_TVal));
			}

			return _Result != 0;
		}

		void wait(_TVal _Expected, memory_order _Order = memory_order_seq_cst) const noexcept {
			const auto _Storage_ptr = addressof(_Storage);
			const auto _Expected_ptr = addressof(_Expected);
			_Int128 _Expected_bytes = reinterpret_cast<const _Int128&>(_Expected);

			for (;;) {
				const _TVal _Observed = load(_Order);
				_Int128 _Observed_bytes = reinterpret_cast<const _Int128&>(_Observed);
				if (_Observed_bytes._Low != _Expected_bytes._Low || _Observed_bytes._High != _Expected_bytes._High) {

					return;
				}

				__std_atomic_wait_indirect(_Storage_ptr, _Expected_ptr, sizeof(_TVal), nullptr,
					&_Atomic_wait_compare_16_bytes, _Atomic_wait_no_timeout);
			}
		}

		void notify_one() noexcept {
			__std_atomic_notify_one_indirect(addressof(_Storage));
		}

		void notify_all() noexcept {
			__std_atomic_notify_all_indirect(addressof(_Storage));
		}

		struct _Int128 {
			alignas(16) long long _Low;
			long long _High;
		};

		typename _Atomic_storage_types<T&>::_TStorage _Storage;
	};

	template <class T, size_t = sizeof(T)>
	struct _Atomic_integral; // not defined

	template <class T>
	struct _Atomic_integral<T, 1> : _Atomic_storage<T> { // atomic integral operations using 1-byte intrinsics
		using _Base = _Atomic_storage<T>;
		using typename _Base::_TVal;

		using _Base::_Base;

		_TVal fetch_add(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			char _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd8,
				_Atomic_address_as<char>(this->_Storage), static_cast<char>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_and(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			char _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedAnd8,
				_Atomic_address_as<char>(this->_Storage), static_cast<char>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_or(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			char _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedOr8,
				_Atomic_address_as<char>(this->_Storage), static_cast<char>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_xor(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			char _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedXor8,
				_Atomic_address_as<char>(this->_Storage), static_cast<char>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal operator++(int) noexcept {
			return static_cast<_TVal>(_InterlockedExchangeAdd8(_Atomic_address_as<char>(this->_Storage), 1));
		}

		_TVal operator++() noexcept {
			unsigned char _Before =
				static_cast<unsigned char>(_InterlockedExchangeAdd8(_Atomic_address_as<char>(this->_Storage), 1));
			++_Before;
			return static_cast<_TVal>(_Before);
		}

		_TVal operator--(int) noexcept {
			return static_cast<_TVal>(_InterlockedExchangeAdd8(_Atomic_address_as<char>(this->_Storage), -1));
		}

		_TVal operator--() noexcept {
			unsigned char _Before =
				static_cast<unsigned char>(_InterlockedExchangeAdd8(_Atomic_address_as<char>(this->_Storage), -1));
			--_Before;
			return static_cast<_TVal>(_Before);
		}
	};

	template <class T>
	struct _Atomic_integral<T, 2> : _Atomic_storage<T> { // atomic integral operations using 2-byte intrinsics
		using _Base = _Atomic_storage<T>;
		using typename _Base::_TVal;

		using _Base::_Base;

		_TVal fetch_add(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			short _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd16,
				_Atomic_address_as<short>(this->_Storage), static_cast<short>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_and(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			short _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedAnd16,
				_Atomic_address_as<short>(this->_Storage), static_cast<short>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_or(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			short _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedOr16,
				_Atomic_address_as<short>(this->_Storage), static_cast<short>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_xor(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			short _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedXor16,
				_Atomic_address_as<short>(this->_Storage), static_cast<short>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal operator++(int) noexcept {
			unsigned short _After =
				static_cast<unsigned short>(_InterlockedIncrement16(_Atomic_address_as<short>(this->_Storage)));
			--_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator++() noexcept {
			return static_cast<_TVal>(_InterlockedIncrement16(_Atomic_address_as<short>(this->_Storage)));
		}

		_TVal operator--(int) noexcept {
			unsigned short _After =
				static_cast<unsigned short>(_InterlockedDecrement16(_Atomic_address_as<short>(this->_Storage)));
			++_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator--() noexcept {
			return static_cast<_TVal>(_InterlockedDecrement16(_Atomic_address_as<short>(this->_Storage)));
		}
	};

	template <class T>
	struct _Atomic_integral<T, 4> : _Atomic_storage<T> { // atomic integral operations using 4-byte intrinsics
		using _Base = _Atomic_storage<T>;
		using typename _Base::_TVal;

		using _Base::_Base;

		_TVal fetch_add(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd,
				_Atomic_address_as<long>(this->_Storage), static_cast<long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_and(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedAnd,
				_Atomic_address_as<long>(this->_Storage), static_cast<long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_or(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedOr,
				_Atomic_address_as<long>(this->_Storage), static_cast<long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_xor(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedXor,
				_Atomic_address_as<long>(this->_Storage), static_cast<long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal operator++(int) noexcept {
			unsigned long _After =
				static_cast<unsigned long>(_InterlockedIncrement(_Atomic_address_as<long>(this->_Storage)));
			--_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator++() noexcept {
			return static_cast<_TVal>(_InterlockedIncrement(_Atomic_address_as<long>(this->_Storage)));
		}

		_TVal operator--(int) noexcept {
			unsigned long _After =
				static_cast<unsigned long>(_InterlockedDecrement(_Atomic_address_as<long>(this->_Storage)));
			++_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator--() noexcept {
			return static_cast<_TVal>(_InterlockedDecrement(_Atomic_address_as<long>(this->_Storage)));
		}
	};

	template <class T>
	struct _Atomic_integral<T, 8> : _Atomic_storage<T> { // atomic integral operations using 8-byte intrinsics
		using _Base = _Atomic_storage<T>;
		using typename _Base::_TVal;

		using _Base::_Base;

		_TVal fetch_add(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd64,
				_Atomic_address_as<long long>(this->_Storage), static_cast<long long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_and(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedAnd64,
				_Atomic_address_as<long long>(this->_Storage), static_cast<long long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_or(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedOr64,
				_Atomic_address_as<long long>(this->_Storage), static_cast<long long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal fetch_xor(const _TVal _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			long long _Result;
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedXor64,
				_Atomic_address_as<long long>(this->_Storage), static_cast<long long>(_Operand));
			return static_cast<_TVal>(_Result);
		}

		_TVal operator++(int) noexcept {
			unsigned long long _After =
				static_cast<unsigned long long>(_InterlockedIncrement64(_Atomic_address_as<long long>(this->_Storage)));
			--_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator++() noexcept {
			return static_cast<_TVal>(_InterlockedIncrement64(_Atomic_address_as<long long>(this->_Storage)));
		}

		_TVal operator--(int) noexcept {
			unsigned long long _After =
				static_cast<unsigned long long>(_InterlockedDecrement64(_Atomic_address_as<long long>(this->_Storage)));
			++_After;
			return static_cast<_TVal>(_After);
		}

		_TVal operator--() noexcept {
			return static_cast<_TVal>(_InterlockedDecrement64(_Atomic_address_as<long long>(this->_Storage)));
		}
	};

	template <size_t _TypeSize>
	inline constexpr bool _Is_always_lock_free = _TypeSize <= 8 && (_TypeSize & (_TypeSize - 1)) == 0;

	template <class T, bool _Is_lock_free = _Is_always_lock_free<sizeof(T)>>
	inline constexpr bool _Deprecate_non_lock_free_volatile = true;

	template <class T>
	[[deprecated("warning STL4030: " "Some operations on volatile-qualified types in the STL are deprecated in C++20. " "You can define _SILENCE_CXX20_VOLATILE_DEPRECATION_WARNING " "or _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS to suppress this warning.")]]
	inline constexpr bool _Deprecate_non_lock_free_volatile<T, false> = true;

	template <class T>
	struct _Atomic_integral_facade : _Atomic_integral<T> {
		// provides operator overloads and other support for atomic integral specializations
		using _Base = _Atomic_integral<T>;
		using difference_type = T;

		using _Base::_Base;

		// _Deprecate_non_lock_free_volatile is unnecessary here.

		// note: const_cast-ing away volatile is safe because all our intrinsics add volatile back on.
		// We make the primary functions non-volatile for better debug codegen, as non-volatile atomics
		// are far more common than volatile ones.
		using _Base::fetch_add;
		T fetch_add(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_add(_Operand);
		}

		T fetch_add(const T _Operand, const memory_order _Order) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_add(_Operand, _Order);
		}

		[[nodiscard]] static T _Negate(const T _Value) noexcept { // returns two's complement negated value of _Value
			return static_cast<T>(0U - static_cast<make_unsigned_t<T>>(_Value));
		}

		T fetch_sub(const T _Operand) noexcept {
			return fetch_add(_Negate(_Operand));
		}

		T fetch_sub(const T _Operand) volatile noexcept {
			return fetch_add(_Negate(_Operand));
		}

		T fetch_sub(const T _Operand, const memory_order _Order) noexcept {
			return fetch_add(_Negate(_Operand), _Order);
		}

		T fetch_sub(const T _Operand, const memory_order _Order) volatile noexcept {
			return fetch_add(_Negate(_Operand), _Order);
		}

		using _Base::fetch_and;
		T fetch_and(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_and(_Operand);
		}

		T fetch_and(const T _Operand, const memory_order _Order) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_and(_Operand, _Order);
		}

		using _Base::fetch_or;
		T fetch_or(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_or(_Operand);
		}

		T fetch_or(const T _Operand, const memory_order _Order) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_or(_Operand, _Order);
		}

		using _Base::fetch_xor;
		T fetch_xor(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_xor(_Operand);
		}

		T fetch_xor(const T _Operand, const memory_order _Order) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_xor(_Operand, _Order);
		}

		using _Base::operator++;
		T operator++(int) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator++(0);
		}

		T operator++() volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator++();
		}

		using _Base::operator--;
		T operator--(int) volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator--(0);
		}

		T operator--() volatile noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator--();
		}

		T operator+=(const T _Operand) noexcept {
			return static_cast<T>(this->_Base::fetch_add(_Operand) + _Operand);
		}

		T operator+=(const T _Operand) volatile noexcept {
			return static_cast<T>(const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_add(_Operand) + _Operand);
		}

		T operator-=(const T _Operand) noexcept {
			return static_cast<T>(fetch_sub(_Operand) - _Operand);
		}

		T operator-=(const T _Operand) volatile noexcept {
			return static_cast<T>(const_cast<_Atomic_integral_facade*>(this)->fetch_sub(_Operand) - _Operand);
		}

		T operator&=(const T _Operand) noexcept {
			return static_cast<T>(this->_Base::fetch_and(_Operand) & _Operand);
		}

		T operator&=(const T _Operand) volatile noexcept {
			return static_cast<T>(const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_and(_Operand) & _Operand);
		}

		T operator|=(const T _Operand) noexcept {
			return static_cast<T>(this->_Base::fetch_or(_Operand) | _Operand);
		}

		T operator|=(const T _Operand) volatile noexcept {
			return static_cast<T>(const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_or(_Operand) | _Operand);
		}

		T operator^=(const T _Operand) noexcept {
			return static_cast<T>(this->_Base::fetch_xor(_Operand) ^ _Operand);
		}

		T operator^=(const T _Operand) volatile noexcept {
			return static_cast<T>(const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_xor(_Operand) ^ _Operand);
		}
	};

	template <class T>
	struct _Atomic_integral_facade<T&> : _Atomic_integral<T&> {
		// provides operator overloads and other support for atomic integral specializations
		using _Base = _Atomic_integral<T&>;
		using difference_type = T;

		using _Base::_Base;

		[[nodiscard]] static T _Negate(const T _Value) noexcept { // returns two's complement negated value of _Value
			return static_cast<T>(0U - static_cast<make_unsigned_t<T>>(_Value));
		}

		T fetch_add(const T _Operand) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_add(_Operand);
		}

		T fetch_add(const T _Operand, const memory_order _Order) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_add(_Operand, _Order);
		}

		T fetch_sub(const T _Operand) const noexcept {
			return fetch_add(_Negate(_Operand));
		}

		T fetch_sub(const T _Operand, const memory_order _Order) const noexcept {
			return fetch_add(_Negate(_Operand), _Order);
		}

		T operator++(int) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator++(0);
		}

		T operator++() const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator++();
		}

		T operator--(int) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator--(0);
		}

		T operator--() const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::operator--();
		}

		T operator+=(const T _Operand) const noexcept {
			return static_cast<T>(fetch_add(_Operand) + _Operand);
		}

		T operator-=(const T _Operand) const noexcept {
			return static_cast<T>(fetch_sub(_Operand) - _Operand);
		}

		T fetch_and(const T _Operand) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_and(_Operand);
		}

		T fetch_and(const T _Operand, const memory_order _Order) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_and(_Operand, _Order);
		}

		T fetch_or(const T _Operand) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_or(_Operand);
		}

		T fetch_or(const T _Operand, const memory_order _Order) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_or(_Operand, _Order);
		}

		T fetch_xor(const T _Operand) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_xor(_Operand);
		}

		T fetch_xor(const T _Operand, const memory_order _Order) const noexcept {
			return const_cast<_Atomic_integral_facade*>(this)->_Base::fetch_xor(_Operand, _Order);
		}

		T operator&=(const T _Operand) const noexcept {
			return static_cast<T>(fetch_and(_Operand) & _Operand);
		}

		T operator|=(const T _Operand) const noexcept {
			return static_cast<T>(fetch_or(_Operand) | _Operand);
		}

		T operator^=(const T _Operand) const noexcept {
			return static_cast<T>(fetch_xor(_Operand) ^ _Operand);
		}
	};

	template <class T>
	struct _Atomic_floating : _Atomic_storage<T> {
		// provides atomic floating-point operations
		using _Base = _Atomic_storage<T>;
		using difference_type = T;

		using _Base::_Base;

		T fetch_add(const T _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			T _Temp{ this->load(memory_order_relaxed) };
			while (!this->compare_exchange_strong(_Temp, _Temp + _Operand, _Order)) { // keep trying
			}

			return _Temp;
		}

		// _Deprecate_non_lock_free_volatile is unnecessary here.

		// note: const_cast-ing away volatile is safe because all our intrinsics add volatile back on.
		// We make the primary functions non-volatile for better debug codegen, as non-volatile atomics
		// are far more common than volatile ones.
		T fetch_add(const T _Operand, const memory_order _Order = memory_order_seq_cst) volatile noexcept {
			return const_cast<_Atomic_floating*>(this)->fetch_add(_Operand, _Order);
		}

		T fetch_sub(const T _Operand, const memory_order _Order = memory_order_seq_cst) noexcept {
			T _Temp{ this->load(memory_order_relaxed) };
			while (!this->compare_exchange_strong(_Temp, _Temp - _Operand, _Order)) { // keep trying
			}

			return _Temp;
		}

		T fetch_sub(const T _Operand, const memory_order _Order = memory_order_seq_cst) volatile noexcept {
			return const_cast<_Atomic_floating*>(this)->fetch_sub(_Operand, _Order);
		}

		T operator+=(const T _Operand) noexcept {
			return fetch_add(_Operand) + _Operand;
		}

		T operator+=(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_floating*>(this)->fetch_add(_Operand) + _Operand;
		}

		T operator-=(const T _Operand) noexcept {
			return fetch_sub(_Operand) - _Operand;
		}

		T operator-=(const T _Operand) volatile noexcept {
			return const_cast<_Atomic_floating*>(this)->fetch_sub(_Operand) - _Operand;
		}
	};

	template <class T>
	struct _Atomic_floating<T&> : _Atomic_storage<T&> {
		// provides atomic floating-point operations
		using _Base = _Atomic_storage<T&>;
		using difference_type = T;

		using _Base::_Base;

		T fetch_add(const T _Operand, const memory_order _Order = memory_order_seq_cst) const noexcept {
			T _Temp{ this->load(memory_order_relaxed) };
			while (!const_cast<_Atomic_floating*>(this)->_Base::compare_exchange_strong(
				_Temp, _Temp + _Operand, _Order)) { // keep trying
			}

			return _Temp;
		}

		T fetch_sub(const T _Operand, const memory_order _Order = memory_order_seq_cst) const noexcept {
			T _Temp{ this->load(memory_order_relaxed) };
			while (!const_cast<_Atomic_floating*>(this)->_Base::compare_exchange_strong(
				_Temp, _Temp - _Operand, _Order)) { // keep trying
			}

			return _Temp;
		}

		T operator+=(const T _Operand) const noexcept {
			return fetch_add(_Operand) + _Operand;
		}

		T operator-=(const T _Operand) const noexcept {
			return fetch_sub(_Operand) - _Operand;
		}
	};

	template <class T>
	struct _Atomic_pointer : _Atomic_storage<T> {
		using _Base = _Atomic_storage<T>;
		using difference_type = ptrdiff_t;

		using _Base::_Base;

		T fetch_add(const ptrdiff_t _Diff, const memory_order _Order = memory_order_seq_cst) noexcept {
			const ptrdiff_t _Shift_bytes =
				static_cast<ptrdiff_t>(static_cast<size_t>(_Diff) * sizeof(remove_pointer_t<T>));
			ptrdiff_t _Result;
#if defined(_M_IX86) || defined(_M_ARM)
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd,
				_Atomic_address_as<long>(this->_Storage), _Shift_bytes);
#else // ^^^ 32 bits / 64 bits vvv
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd64,
				_Atomic_address_as<long long>(this->_Storage), _Shift_bytes);
#endif // hardware
			return reinterpret_cast<T>(_Result);
		}

		// _Deprecate_non_lock_free_volatile is unnecessary here.

		T fetch_add(const ptrdiff_t _Diff) volatile noexcept {
			return const_cast<_Atomic_pointer*>(this)->fetch_add(_Diff);
		}

		T fetch_add(const ptrdiff_t _Diff, const memory_order _Order) volatile noexcept {
			return const_cast<_Atomic_pointer*>(this)->fetch_add(_Diff, _Order);
		}

		T fetch_sub(const ptrdiff_t _Diff) volatile noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)));
		}

		T fetch_sub(const ptrdiff_t _Diff) noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)));
		}

		T fetch_sub(const ptrdiff_t _Diff, const memory_order _Order) volatile noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)), _Order);
		}

		T fetch_sub(const ptrdiff_t _Diff, const memory_order _Order) noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)), _Order);
		}

		T operator++(int) volatile noexcept {
			return fetch_add(1);
		}

		T operator++(int) noexcept {
			return fetch_add(1);
		}

		T operator++() volatile noexcept {
			return fetch_add(1) + 1;
		}

		T operator++() noexcept {
			return fetch_add(1) + 1;
		}

		T operator--(int) volatile noexcept {
			return fetch_add(-1);
		}

		T operator--(int) noexcept {
			return fetch_add(-1);
		}

		T operator--() volatile noexcept {
			return fetch_add(-1) - 1;
		}

		T operator--() noexcept {
			return fetch_add(-1) - 1;
		}

		T operator+=(const ptrdiff_t _Diff) volatile noexcept {
			return fetch_add(_Diff) + _Diff;
		}

		T operator+=(const ptrdiff_t _Diff) noexcept {
			return fetch_add(_Diff) + _Diff;
		}

		T operator-=(const ptrdiff_t _Diff) volatile noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff))) - _Diff;
		}

		T operator-=(const ptrdiff_t _Diff) noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff))) - _Diff;
		}
	};

	template <class T>
	struct _Atomic_pointer<T&> : _Atomic_storage<T&> {
		using _Base = _Atomic_storage<T&>;
		using difference_type = ptrdiff_t;

		using _Base::_Base;

		T fetch_add(const ptrdiff_t _Diff, const memory_order _Order = memory_order_seq_cst) const noexcept {
			const ptrdiff_t _Shift_bytes =
				static_cast<ptrdiff_t>(static_cast<size_t>(_Diff) * sizeof(remove_pointer_t<T>));
			ptrdiff_t _Result;
#if defined(_M_IX86) || defined(_M_ARM)
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd,
				_Atomic_address_as<long>(this->_Storage), _Shift_bytes);
#else // ^^^ 32 bits / 64 bits vvv
			_ATOMIC_CHOOSE_INTRINSIC(static_cast<unsigned int>(_Order), _Result, _InterlockedExchangeAdd64,
				_Atomic_address_as<long long>(this->_Storage), _Shift_bytes);
#endif // hardware
			return reinterpret_cast<T>(_Result);
		}

		T fetch_sub(const ptrdiff_t _Diff) const noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)));
		}

		T fetch_sub(const ptrdiff_t _Diff, const memory_order _Order) const noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff)), _Order);
		}

		T operator++(int) const noexcept {
			return fetch_add(1);
		}

		T operator++() const noexcept {
			return fetch_add(1) + 1;
		}

		T operator--(int) const noexcept {
			return fetch_add(-1);
		}

		T operator--() const noexcept {
			return fetch_add(-1) - 1;
		}

		T operator+=(const ptrdiff_t _Diff) const noexcept {
			return fetch_add(_Diff) + _Diff;
		}

		T operator-=(const ptrdiff_t _Diff) const noexcept {
			return fetch_add(static_cast<ptrdiff_t>(0 - static_cast<size_t>(_Diff))) - _Diff;
		}
	};

	template <class _TVal, class T = _TVal>
	using _Choose_atomic_base2_t =
		typename _Select<is_integral_v<_TVal> && !is_same_v<bool, _TVal>>::template _Apply<_Atomic_integral_facade<T>,
		typename _Select<is_pointer_v<_TVal>&& is_object_v<remove_pointer_t<_TVal>>>::template _Apply<
		_Atomic_pointer<T>, _Atomic_storage<T>>>;

	template <class _TVal, class T = _TVal>
	using _Choose_atomic_base_t = typename _Select<is_floating_point_v<_TVal>>::template _Apply<_Atomic_floating<T>,
		_Choose_atomic_base2_t<_TVal, T>>;
}

export namespace std
{
	template <class T>
	struct atomic : _Choose_atomic_base_t<T>
	{
	private:
		using _Base = _Choose_atomic_base_t<T>;

	public:
		static_assert(is_trivially_copyable_v<T>&& is_copy_constructible_v<T>&& is_move_constructible_v<T>
			&& is_copy_assignable_v<T>&& is_move_assignable_v<T>,
			"atomic<T> requires T to be trivially copyable, copy constructible, move constructible, copy assignable, "
			"and move assignable.");

		using value_type = T;

		using _Base::_Base;

		constexpr atomic() noexcept(is_nothrow_default_constructible_v<T>) : _Base() {}

		atomic(const atomic&) = delete;
		atomic& operator=(const atomic&) = delete;

		static constexpr bool is_always_lock_free = _Is_always_lock_free<sizeof(T)>;

		[[nodiscard]] bool is_lock_free() const volatile noexcept {
			constexpr bool _Result = sizeof(T) <= 8 && (sizeof(T) & sizeof(T) - 1) == 0;
			return _Result;
		}

		[[nodiscard]] bool is_lock_free() const noexcept {
			return static_cast<const volatile atomic*>(this)->is_lock_free();
		}

		T operator=(const T _Value) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			this->store(_Value);
			return _Value;
		}

		T operator=(const T _Value) noexcept {
			this->store(_Value);
			return _Value;
		}

		using _Base::store;
		void store(const T _Value) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			const_cast<atomic*>(this)->_Base::store(_Value);
		}

		void store(const T _Value, const memory_order _Order) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			const_cast<atomic*>(this)->_Base::store(_Value, _Order);
		}

		using _Base::load;
		[[nodiscard]] T load() const volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<const atomic*>(this)->_Base::load();
		}

		[[nodiscard]] T load(const memory_order _Order) const volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<const atomic*>(this)->_Base::load(_Order);
		}

		using _Base::exchange;
		T exchange(const T _Value) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<atomic*>(this)->_Base::exchange(_Value);
		}

		T exchange(const T _Value, const memory_order _Order) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<atomic*>(this)->_Base::exchange(_Value, _Order);
		}

		using _Base::compare_exchange_strong;
		bool compare_exchange_strong(T& _Expected, const T _Desired) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<atomic*>(this)->_Base::compare_exchange_strong(_Expected, _Desired);
		}

		bool compare_exchange_strong(T& _Expected, const T _Desired, const memory_order _Order) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return const_cast<atomic*>(this)->_Base::compare_exchange_strong(_Expected, _Desired, _Order);
		}

		bool compare_exchange_strong(T& _Expected, const T _Desired, const memory_order _Success,
			const memory_order _Failure) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return this->compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
		}

		bool compare_exchange_strong(
			T& _Expected, const T _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
			return this->compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
		}

		bool compare_exchange_weak(T& _Expected, const T _Desired) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return this->compare_exchange_strong(_Expected, _Desired);
		}

		bool compare_exchange_weak(T& _Expected, const T _Desired) noexcept {
			return this->compare_exchange_strong(_Expected, _Desired);
		}

		bool compare_exchange_weak(T& _Expected, const T _Desired, const memory_order _Order) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return this->compare_exchange_strong(_Expected, _Desired, _Order);
		}

		bool compare_exchange_weak(T& _Expected, const T _Desired, const memory_order _Order) noexcept {
			return this->compare_exchange_strong(_Expected, _Desired, _Order);
		}

		bool compare_exchange_weak(T& _Expected, const T _Desired, const memory_order _Success,
			const memory_order _Failure) volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return this->compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
		}

		bool compare_exchange_weak(
			T& _Expected, const T _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
			return this->compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
		}

		using _Base::wait;
		void wait(const T _Expected, const memory_order _Order = memory_order_seq_cst) const volatile noexcept {
			const_cast<const atomic*>(this)->_Base::wait(_Expected, _Order);
		}

		using _Base::notify_one;
		void notify_one() volatile noexcept {
			const_cast<atomic*>(this)->_Base::notify_one();
		}

		using _Base::notify_all;
		void notify_all() volatile noexcept {
			const_cast<atomic*>(this)->_Base::notify_all();
		}

		operator T() const volatile noexcept {
			static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
			return this->load();
		}

		operator T() const noexcept {
			return this->load();
		}
	};

	template <class T>
	atomic(T) -> atomic<T>;

	template <class T>
		struct atomic_ref : _Choose_atomic_base_t<T, T&> { // atomic reference
		private:
			using _Base = _Choose_atomic_base_t<T, T&>;

		public:
			static_assert(is_trivially_copyable_v<T>, "atomic_ref<T> requires T to be trivially copyable.");

			using value_type = T;

			explicit atomic_ref(T& _Value) noexcept /* strengthened */ : _Base(_Value) {
				if constexpr (_Is_potentially_lock_free) {
					_Check_alignment(_Value);
				}
				else {
					this->_Init_spinlock_for_ref();
				}
			}

			atomic_ref(const atomic_ref&) noexcept = default;

			atomic_ref& operator=(const atomic_ref&) = delete;

			static constexpr bool is_always_lock_free = _Is_always_lock_free<sizeof(T)>;

			static constexpr bool _Is_potentially_lock_free =
				sizeof(T) <= 2 * sizeof(void*) && (sizeof(T) & (sizeof(T) - 1)) == 0;

			static constexpr size_t required_alignment = _Is_potentially_lock_free ? sizeof(T) : alignof(T);

			[[nodiscard]] bool is_lock_free() const noexcept {
				if constexpr (is_always_lock_free) {
					return true;
				}
				else {
					return __std_atomic_has_cmpxchg16b() != 0;
				}
			}

			void store(const T _Value) const noexcept {
				const_cast<atomic_ref*>(this)->_Base::store(_Value);
			}

			void store(const T _Value, const memory_order _Order) const noexcept {
				const_cast<atomic_ref*>(this)->_Base::store(_Value, _Order);
			}

			T operator=(const T _Value) const noexcept {
				store(_Value);
				return _Value;
			}

			T exchange(const T _Value) const noexcept {
				return const_cast<atomic_ref*>(this)->_Base::exchange(_Value);
			}

			T exchange(const T _Value, const memory_order _Order) const noexcept {
				return const_cast<atomic_ref*>(this)->_Base::exchange(_Value, _Order);
			}

			bool compare_exchange_strong(T& _Expected, const T _Desired) const noexcept {
				return const_cast<atomic_ref*>(this)->_Base::compare_exchange_strong(_Expected, _Desired);
			}

			bool compare_exchange_strong(T& _Expected, const T _Desired, const memory_order _Order) const noexcept {
				return const_cast<atomic_ref*>(this)->_Base::compare_exchange_strong(_Expected, _Desired, _Order);
			}

			bool compare_exchange_strong(
				T& _Expected, const T _Desired, const memory_order _Success, const memory_order _Failure) const noexcept {
				return compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
			}

			bool compare_exchange_weak(T& _Expected, const T _Desired) const noexcept {
				return compare_exchange_strong(_Expected, _Desired);
			}

			bool compare_exchange_weak(T& _Expected, const T _Desired, const memory_order _Order) const noexcept {
				return compare_exchange_strong(_Expected, _Desired, _Order);
			}

			bool compare_exchange_weak(
				T& _Expected, const T _Desired, const memory_order _Success, const memory_order _Failure) const noexcept {
				return compare_exchange_strong(_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
			}

			operator T() const noexcept {
				return this->load();
			}

			void notify_one() const noexcept {
				const_cast<atomic_ref*>(this)->_Base::notify_one();
			}

			void notify_all() const noexcept {
				const_cast<atomic_ref*>(this)->_Base::notify_all();
			}

		private:
			static void _Check_alignment([[maybe_unused]] const T& _Value) {
				_ATOMIC_REF_CHECK_ALIGNMENT(
					(reinterpret_cast<size_t>(addressof(_Value)) & (required_alignment - 1)) == 0,
					"atomic_ref underlying object is not aligned as required_alignment");
			}
	};

	template <class T>
		[[nodiscard]] bool atomic_is_lock_free(const volatile atomic<T>* _Mem) noexcept {
		return _Mem->is_lock_free();
	}

	template <class T>
		[[nodiscard]] bool atomic_is_lock_free(const atomic<T>* _Mem) noexcept {
		return _Mem->is_lock_free();
	}

	template <class T>
		void atomic_store(volatile atomic<T>* const _Mem, const _Identity_t<T> _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		_Mem->store(_Value);
	}

	template <class T>
		void atomic_store(atomic<T>* const _Mem, const _Identity_t<T> _Value) noexcept {
		_Mem->store(_Value);
	}

	template <class T>
		void atomic_store_explicit(
			volatile atomic<T>* const _Mem, const _Identity_t<T> _Value, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		_Mem->store(_Value, _Order);
	}

	template <class T>
		void atomic_store_explicit(atomic<T>* const _Mem, const _Identity_t<T> _Value, const memory_order _Order) noexcept {
		_Mem->store(_Value, _Order);
	}

	template <class T>
		[[nodiscard]] T atomic_load(const volatile atomic<T>* const _Mem) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->load();
	}

	template <class T>
		[[nodiscard]] T atomic_load(const atomic<T>* const _Mem) noexcept {
		return _Mem->load();
	}

	template <class T>
		[[nodiscard]] T atomic_load_explicit(const volatile atomic<T>* const _Mem, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->load(_Order);
	}

	template <class T>
		[[nodiscard]] T atomic_load_explicit(const atomic<T>* const _Mem, const memory_order _Order) noexcept {
		return _Mem->load(_Order);
	}

	template <class T>
		T atomic_exchange(volatile atomic<T>* const _Mem, const _Identity_t<T> _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->exchange(_Value);
	}

	template <class T>
		T atomic_exchange(atomic<T>* const _Mem, const _Identity_t<T> _Value) noexcept {
		return _Mem->exchange(_Value);
	}

	template <class T>
		T atomic_exchange_explicit(
			volatile atomic<T>* const _Mem, const _Identity_t<T> _Value, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->exchange(_Value, _Order);
	}

	template <class T>
		T atomic_exchange_explicit(
			atomic<T>* const _Mem, const _Identity_t<T> _Value, const memory_order _Order) noexcept {
		return _Mem->exchange(_Value, _Order);
	}

	template <class T>
		bool atomic_compare_exchange_strong(
			volatile atomic<T>* const _Mem, _Identity_t<T>* const _Expected, const _Identity_t<T> _Desired) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->compare_exchange_strong(*_Expected, _Desired);
	}

	template <class T>
		bool atomic_compare_exchange_strong(
			atomic<T>* const _Mem, _Identity_t<T>* const _Expected, const _Identity_t<T> _Desired) noexcept {
		return _Mem->compare_exchange_strong(*_Expected, _Desired);
	}

	template <class T>
		bool atomic_compare_exchange_strong_explicit(volatile atomic<T>* const _Mem, _Identity_t<T>* const _Expected,
			const _Identity_t<T> _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->compare_exchange_strong(*_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
	}

	template <class T>
		bool atomic_compare_exchange_strong_explicit(atomic<T>* const _Mem, _Identity_t<T>* const _Expected,
			const _Identity_t<T> _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
		return _Mem->compare_exchange_strong(*_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
	}

	template <class T>
		bool atomic_compare_exchange_weak(
			volatile atomic<T>* const _Mem, _Identity_t<T>* const _Expected, const _Identity_t<T> _Desired) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->compare_exchange_strong(*_Expected, _Desired);
	}

	template <class T>
		bool atomic_compare_exchange_weak(
			atomic<T>* const _Mem, _Identity_t<T>* const _Expected, const _Identity_t<T> _Desired) noexcept {
		return _Mem->compare_exchange_strong(*_Expected, _Desired);
	}

	template <class T>
		bool atomic_compare_exchange_weak_explicit(volatile atomic<T>* const _Mem, _Identity_t<T>* const _Expected,
			const _Identity_t<T> _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->compare_exchange_strong(*_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
	}

	template <class T>
		bool atomic_compare_exchange_weak_explicit(atomic<T>* const _Mem, _Identity_t<T>* const _Expected,
			const _Identity_t<T> _Desired, const memory_order _Success, const memory_order _Failure) noexcept {
		return _Mem->compare_exchange_strong(*_Expected, _Desired, _Combine_cas_memory_orders(_Success, _Failure));
	}

	template <class T>
		T atomic_fetch_add(volatile atomic<T>* _Mem, const typename atomic<T>::difference_type _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_add(_Value);
	}

	template <class T>
		T atomic_fetch_add(atomic<T>* _Mem, const typename atomic<T>::difference_type _Value) noexcept {
		return _Mem->fetch_add(_Value);
	}

	template <class T>
		T atomic_fetch_add_explicit(volatile atomic<T>* _Mem, const typename atomic<T>::difference_type _Value,
			const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_add(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_add_explicit(
			atomic<T>* _Mem, const typename atomic<T>::difference_type _Value, const memory_order _Order) noexcept {
		return _Mem->fetch_add(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_sub(volatile atomic<T>* _Mem, const typename atomic<T>::difference_type _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_sub(_Value);
	}

	template <class T>
		T atomic_fetch_sub(atomic<T>* _Mem, const typename atomic<T>::difference_type _Value) noexcept {
		return _Mem->fetch_sub(_Value);
	}

	template <class T>
		T atomic_fetch_sub_explicit(volatile atomic<T>* _Mem, const typename atomic<T>::difference_type _Value,
			const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_sub(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_sub_explicit(
			atomic<T>* _Mem, const typename atomic<T>::difference_type _Value, const memory_order _Order) noexcept {
		return _Mem->fetch_sub(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_and(volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_and(_Value);
	}

	template <class T>
		T atomic_fetch_and(atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		return _Mem->fetch_and(_Value);
	}

	template <class T>
		T atomic_fetch_and_explicit(
			volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_and(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_and_explicit(
			atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		return _Mem->fetch_and(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_or(volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_or(_Value);
	}

	template <class T>
		T atomic_fetch_or(atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		return _Mem->fetch_or(_Value);
	}

	template <class T>
		T atomic_fetch_or_explicit(
			volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_or(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_or_explicit(
			atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		return _Mem->fetch_or(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_xor(volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_xor(_Value);
	}

	template <class T>
		T atomic_fetch_xor(atomic<T>* _Mem, const typename atomic<T>::value_type _Value) noexcept {
		return _Mem->fetch_xor(_Value);
	}

	template <class T>
		T atomic_fetch_xor_explicit(
			volatile atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		static_assert(_Deprecate_non_lock_free_volatile<T>, "Never fails");
		return _Mem->fetch_xor(_Value, _Order);
	}

	template <class T>
		T atomic_fetch_xor_explicit(
			atomic<T>* _Mem, const typename atomic<T>::value_type _Value, const memory_order _Order) noexcept {
		return _Mem->fetch_xor(_Value, _Order);
	}

	template <class T>
		void atomic_wait(const volatile atomic<T>* const _Mem, const typename atomic<T>::value_type _Expected) noexcept {
		_Mem->wait(_Expected);
	}

	template <class T>
		void atomic_wait(const atomic<T>* const _Mem, const typename atomic<T>::value_type _Expected) noexcept {
		_Mem->wait(_Expected);
	}

	template <class T>
		void atomic_wait_explicit(const volatile atomic<T>* const _Mem, const typename atomic<T>::value_type _Expected,
			const memory_order _Order) noexcept {
		_Mem->wait(_Expected, _Order);
	}

	template <class T>
		void atomic_wait_explicit(const atomic<T>* const _Mem, const typename atomic<T>::value_type _Expected,
			const memory_order _Order) noexcept {
		_Mem->wait(_Expected, _Order);
	}

	template <class T>
		void atomic_notify_one(volatile atomic<T>* const _Mem) noexcept {
		_Mem->notify_one();
	}

	template <class T>
		void atomic_notify_one(atomic<T>* const _Mem) noexcept {
		_Mem->notify_one();
	}

	template <class T>
		void atomic_notify_all(volatile atomic<T>* const _Mem) noexcept {
		_Mem->notify_all();
	}

	template <class T>
		void atomic_notify_all(atomic<T>* const _Mem) noexcept {
		_Mem->notify_all();
	}

	using atomic_bool = atomic<bool>;

	using atomic_char = atomic<char>;
	using atomic_schar = atomic<signed char>;
	using atomic_uchar = atomic<unsigned char>;
	using atomic_short = atomic<short>;
	using atomic_ushort = atomic<unsigned short>;
	using atomic_int = atomic<int>;
	using atomic_uint = atomic<unsigned int>;
	using atomic_long = atomic<long>;
	using atomic_ulong = atomic<unsigned long>;
	using atomic_llong = atomic<long long>;
	using atomic_ullong = atomic<unsigned long long>;

	using atomic_char8_t = atomic<char8_t>;
	using atomic_char16_t = atomic<char16_t>;
	using atomic_char32_t = atomic<char32_t>;

	using atomic_int8_t = atomic<int8_t>;
	using atomic_uint8_t = atomic<uint8_t>;
	using atomic_int16_t = atomic<int16_t>;
	using atomic_uint16_t = atomic<uint16_t>;
	using atomic_int32_t = atomic<int32_t>;
	using atomic_uint32_t = atomic<uint32_t>;
	using atomic_int64_t = atomic<int64_t>;
	using atomic_uint64_t = atomic<uint64_t>;

	using atomic_size_t = atomic<size_t>;
	using atomic_ptrdiff_t = atomic<ptrdiff_t>;

	// Though there are CMPXCHG8B and CMPXCHG16B,
	// the largest atomics with a full set of efficient operations are pointer-sized.
	using atomic_signed_lock_free = atomic_ptrdiff_t;
	using atomic_unsigned_lock_free = atomic_size_t;

#define ATOMIC_FLAG_INIT \
    {}

	struct atomic_flag { // flag with test-and-set semantics
		[[nodiscard]] bool test(const memory_order _Order = memory_order_seq_cst) const noexcept {
			return _Storage.load(_Order) != 0;
		}

		[[nodiscard]] bool test(const memory_order _Order = memory_order_seq_cst) const volatile noexcept {
			return _Storage.load(_Order) != 0;
		}

		bool test_and_set(const memory_order _Order = memory_order_seq_cst) noexcept {
			return _Storage.exchange(true, _Order) != 0;
		}

		bool test_and_set(const memory_order _Order = memory_order_seq_cst) volatile noexcept {
			return _Storage.exchange(true, _Order) != 0;
		}

		void clear(const memory_order _Order = memory_order_seq_cst) noexcept {
			_Storage.store(false, _Order);
		}

		void clear(const memory_order _Order = memory_order_seq_cst) volatile noexcept {
			_Storage.store(false, _Order);
		}

		constexpr atomic_flag() noexcept = default;

		void wait(const bool _Expected, const memory_order _Order = memory_order_seq_cst) const noexcept {
			_Storage.wait(static_cast<decltype(_Storage)::value_type>(_Expected), _Order);
		}

		void wait(const bool _Expected, const memory_order _Order = memory_order_seq_cst) const volatile noexcept {
			_Storage.wait(static_cast<decltype(_Storage)::value_type>(_Expected), _Order);
		}

		void notify_one() noexcept {
			_Storage.notify_one();
		}

		void notify_one() volatile noexcept {
			_Storage.notify_one();
		}

		void notify_all() noexcept {
			_Storage.notify_all();
		}

		void notify_all() volatile noexcept {
			_Storage.notify_all();
		}

		atomic<long> _Storage;
	};

	[[nodiscard]] inline bool atomic_flag_test(const volatile atomic_flag* const _Flag) noexcept {
		return _Flag->test();
	}

	[[nodiscard]] inline bool atomic_flag_test(const atomic_flag* const _Flag) noexcept {
		return _Flag->test();
	}

	[[nodiscard]] inline bool atomic_flag_test_explicit(
		const volatile atomic_flag* const _Flag, const memory_order _Order) noexcept {
		return _Flag->test(_Order);
	}

	[[nodiscard]] inline bool atomic_flag_test_explicit(
		const atomic_flag* const _Flag, const memory_order _Order) noexcept {
		return _Flag->test(_Order);
	}

	inline bool atomic_flag_test_and_set(atomic_flag* const _Flag) noexcept {
		return _Flag->test_and_set();
	}

	inline bool atomic_flag_test_and_set(volatile atomic_flag* const _Flag) noexcept {
		return _Flag->test_and_set();
	}

	inline bool atomic_flag_test_and_set_explicit(
		atomic_flag* const _Flag, const memory_order _Order) noexcept {
		return _Flag->test_and_set(_Order);
	}

	inline bool atomic_flag_test_and_set_explicit(
		volatile atomic_flag* const _Flag, const memory_order _Order) noexcept {
		return _Flag->test_and_set(_Order);
	}

	inline void atomic_flag_clear(atomic_flag* const _Flag) noexcept {
		_Flag->clear();
	}

	inline void atomic_flag_clear(volatile atomic_flag* const _Flag) noexcept {
		_Flag->clear();
	}

	inline void atomic_flag_clear_explicit(atomic_flag* const _Flag, const memory_order _Order) noexcept {
		_Flag->clear(_Order);
	}

	inline void atomic_flag_clear_explicit(
		volatile atomic_flag* const _Flag, const memory_order _Order) noexcept {
		_Flag->clear(_Order);
	}

	inline void atomic_flag_wait(const volatile atomic_flag* const _Flag, const bool _Expected) noexcept {
		return _Flag->wait(_Expected);
	}

	inline void atomic_flag_wait(const atomic_flag* const _Flag, const bool _Expected) noexcept {
		return _Flag->wait(_Expected);
	}

	inline void atomic_flag_wait_explicit(
		const volatile atomic_flag* const _Flag, const bool _Expected, const memory_order _Order) noexcept {
		return _Flag->wait(_Expected, _Order);
	}

	inline void atomic_flag_wait_explicit(
		const atomic_flag* const _Flag, const bool _Expected, const memory_order _Order) noexcept {
		return _Flag->wait(_Expected, _Order);
	}

	inline void atomic_flag_notify_one(volatile atomic_flag* const _Flag) noexcept {
		return _Flag->notify_one();
	}

	inline void atomic_flag_notify_one(atomic_flag* const _Flag) noexcept {
		return _Flag->notify_one();
	}

	inline void atomic_flag_notify_all(volatile atomic_flag* const _Flag) noexcept {
		return _Flag->notify_all();
	}

	inline void atomic_flag_notify_all(atomic_flag* const _Flag) noexcept {
		return _Flag->notify_all();
	}
	template <class T>
	class _Locked_pointer {
	public:
		static_assert(alignof(T) >= (1 << 2), "2 low order bits are needed by _Locked_pointer");
		static constexpr size_t _Lock_mask = 3;
		static constexpr size_t _Not_locked = 0;
		static constexpr size_t _Locked_notify_not_needed = 1;
		static constexpr size_t _Locked_notify_needed = 2;
		static constexpr size_t _Ptr_value_mask = ~_Lock_mask;

		constexpr _Locked_pointer() noexcept : _Storage{} {}
		explicit _Locked_pointer(T* const _Ptr) noexcept : _Storage{ reinterpret_cast<size_t>(_Ptr) } {}

		_Locked_pointer(const _Locked_pointer&) = delete;
		_Locked_pointer& operator=(const _Locked_pointer&) = delete;

		[[nodiscard]] T* _Lock_and_load() noexcept {
			size_t _Rep = _Storage.load(memory_order_relaxed);
			for (;;) {
				switch (_Rep & _Lock_mask) {
				case _Not_locked: // Can try to lock now
					if (_Storage.compare_exchange_weak(_Rep, _Rep | _Locked_notify_not_needed)) {
						return reinterpret_cast<T*>(_Rep);
					}
					_mm_pause();
					break;

				case _Locked_notify_not_needed: // Try to set "notify needed" and wait
					if (!_Storage.compare_exchange_weak(_Rep, (_Rep & _Ptr_value_mask) | _Locked_notify_needed)) {
						// Failed to set notify needed flag, try again
						_mm_pause();
						break;
					}
					_Rep = (_Rep & _Ptr_value_mask) | _Locked_notify_needed;
					[[fallthrough]];

				case _Locked_notify_needed: // "Notify needed" is already set, just wait
					_Storage.wait(_Rep, memory_order_relaxed);
					_Rep = _Storage.load(memory_order_relaxed);
					break;

				default: // Unrecognized bit pattern
					return 0;
				}
			}
		}

		void _Store_and_unlock(T* const _Value) noexcept {
			const auto _Rep = _Storage.exchange(reinterpret_cast<size_t>(_Value));
			if ((_Rep & _Lock_mask) == _Locked_notify_needed) {
				// As we don't count waiters, every waiter is notified, and then some may re-request notification
				_Storage.notify_all();
			}
		}

		[[nodiscard]] T* _Unsafe_load_relaxed() const noexcept {
			return reinterpret_cast<T*>(_Storage.load(memory_order_relaxed));
		}

	private:
		atomic<size_t> _Storage;
	};
}