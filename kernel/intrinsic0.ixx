export module intrinsic0;
export extern "C"
{
		unsigned char _BitScanForward(unsigned long* _Index, unsigned long _Mask);
		unsigned char _BitScanForward64(unsigned long* _Index, unsigned __int64 _Mask);
		unsigned char _BitScanReverse(unsigned long* _Index, unsigned long _Mask);
		unsigned char _BitScanReverse64(unsigned long* _Index, unsigned __int64 _Mask);
		unsigned char _bittest(long const*, long);
		long _InterlockedAnd(long volatile* _Value, long _Mask);
		short _InterlockedAnd16(short volatile* _Value, short _Mask);
		__int64 _InterlockedAnd64(__int64 volatile* _Value, __int64 _Mask);
		__int64 _interlockedand64(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedAnd8(char volatile* _Value, char _Mask);
		long _InterlockedCompareExchange(long volatile* _Destination, long _Exchange, long _Comparand);
		long _InterlockedCompareExchange(long volatile* _Destination, long _Exchange, long _Comparand);
		short _InterlockedCompareExchange16(short volatile* _Destination, short _Exchange, short _Comparand);
		__int64 _InterlockedCompareExchange64(__int64 volatile* _Destination, __int64 _Exchange, __int64 _Comparand);
		char _InterlockedCompareExchange8(char volatile* _Destination, char _Exchange, char _Comparand);
		long _InterlockedDecrement(long volatile* _Addend);
		long _InterlockedDecrement(long volatile* _Addend);
		short _InterlockedDecrement16(short volatile* _Addend);
		__int64 _InterlockedDecrement64(__int64 volatile* _Addend);
		__int64 _interlockeddecrement64(__int64 volatile* _Addend);
		long _InterlockedExchange(long volatile* _Target, long _Value);
		long _InterlockedExchange(long volatile* _Target, long _Value);
		short _InterlockedExchange16(short volatile* _Target, short _Value);
		__int64 _InterlockedExchange64(__int64 volatile* _Target, __int64 _Value);
		__int64 _interlockedexchange64(__int64 volatile* _Target, __int64 _Value);
		char _InterlockedExchange8(char volatile* _Target, char _Value);
		long _InterlockedExchangeAdd(long volatile* _Addend, long _Value);
		short _InterlockedExchangeAdd16(short volatile* _Addend, short _Value);
		__int64 _InterlockedExchangeAdd64(__int64 volatile* _Addend, __int64 _Value);
		__int64 _interlockedexchangeadd64(__int64 volatile* _Addend, __int64 _Value);
		char _InterlockedExchangeAdd8(char volatile* _Addend, char _Value);
		long _InterlockedIncrement(long volatile* _Addend);
		long _InterlockedIncrement(long volatile* _Addend);
		short _InterlockedIncrement16(short volatile* _Addend);
		__int64 _InterlockedIncrement64(__int64 volatile* _Addend);
		__int64 _interlockedincrement64(__int64 volatile* _Addend);
		long _InterlockedIncrement_nf(long volatile* _Addend);
		long _InterlockedOr(long volatile* _Value, long _Mask);
		short _InterlockedOr16(short volatile* _Value, short _Mask);
		__int64 _InterlockedOr64(__int64 volatile* _Value, __int64 _Mask);
		__int64 _interlockedor64(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedOr8(char volatile* _Value, char _Mask);
		long _InterlockedXor(long volatile* _Value, long _Mask);
		short _InterlockedXor16(short volatile* _Value, short _Mask);
		__int64 _InterlockedXor64(__int64 volatile* _Value, __int64 _Mask);
		__int64 _interlockedxor64(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedXor8(char volatile* _Value, char _Mask);
		void _ReadWriteBarrier();
		__int16 __iso_volatile_load16(const volatile __int16*);
		__int32 __iso_volatile_load32(const volatile __int32*);
		__int64 __iso_volatile_load64(const volatile __int64*);
		__int8 __iso_volatile_load8(const volatile __int8*);
		void __iso_volatile_store16(volatile __int16*, __int16);
		void __iso_volatile_store32(volatile __int32*, __int32);
		void __iso_volatile_store64(volatile __int64*, __int64);
		void __iso_volatile_store8(volatile __int8*, __int8);
		void _mm_pause();
		unsigned int __lzcnt(unsigned int);
		unsigned short __lzcnt16(unsigned short);
		unsigned __int64 __lzcnt64(unsigned __int64);
		unsigned int __popcnt(unsigned int);
		unsigned short __popcnt16(unsigned short);
		unsigned __int64 __popcnt64(unsigned __int64);
		unsigned int __cdecl _rotl(unsigned int _Value, int shift);
		unsigned short __cdecl _rotl16(unsigned short _Value, unsigned char shift);
		unsigned __int64 __cdecl _rotl64(unsigned __int64 _Value, int shift);
		unsigned char __cdecl _rotl8(unsigned char _Value, unsigned char shift);
		unsigned int __cdecl _rotr(unsigned int _Value, int shift);
		unsigned short __cdecl _rotr16(unsigned short _Value, unsigned char shift);
		unsigned __int64 __cdecl _rotr64(unsigned __int64 _Value, int shift);
		unsigned char __cdecl _rotr8(unsigned char _Value, unsigned char shift);
		unsigned __int64 __shiftleft128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char shift);
		unsigned __int64 __shiftright128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char shift);
		unsigned __int64 _umul128(unsigned __int64 _Multiplier, unsigned __int64 _Multiplicand, unsigned __int64* _HighProduct);
		unsigned __int64 __cdecl _udiv128(unsigned __int64 _HighDividend, unsigned __int64 _LowDividend, unsigned __int64 _Divisor, unsigned __int64* _Remainder);
		unsigned char __cdecl _addcarry_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64*);
		unsigned char __cdecl _subborrow_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64*);
		double __ceil(double);
		float __ceilf(float);
		double __floor(double);
		float __floorf(float);
		double __round(double);
		float __roundf(float);
		double __trunc(double);
		float __truncf(float);
		double __copysign(double, double);
		float __copysignf(float, float);
		unsigned __signbitvalue(double);
		unsigned __signbitvaluef(float);
}