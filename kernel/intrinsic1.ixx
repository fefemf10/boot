export module intrinsic1;

export extern "C"
{
		void* _AddressOfReturnAddress();
		int _cvt_dtoi_fast(double);
		int _cvt_dtoi_sat(double);
		int _cvt_dtoi_sent(double);
		long long _cvt_dtoll_fast(double);
		long long _cvt_dtoll_sat(double);
		long long _cvt_dtoll_sent(double);
		unsigned _cvt_dtoui_fast(double);
		unsigned _cvt_dtoui_sat(double);
		unsigned _cvt_dtoui_sent(double);
		unsigned long long _cvt_dtoull_fast(double);
		unsigned long long _cvt_dtoull_sat(double);
		unsigned long long _cvt_dtoull_sent(double);
		int _cvt_ftoi_fast(float);
		int _cvt_ftoi_sat(float);
		int _cvt_ftoi_sent(float);
		long long _cvt_ftoll_fast(float);
		long long _cvt_ftoll_sat(float);
		long long _cvt_ftoll_sent(float);
		unsigned _cvt_ftoui_fast(float);
		unsigned _cvt_ftoui_sat(float);
		unsigned _cvt_ftoui_sent(float);
		unsigned long long _cvt_ftoull_fast(float);
		unsigned long long _cvt_ftoull_sat(float);
		unsigned long long _cvt_ftoull_sent(float);
		void _HybridGenerateThunks(void*, unsigned);
		long _interlockedadd(long volatile* _Addend, long _Value);
		__int64 _interlockedadd64(__int64 volatile* _Addend, __int64 _Value);
		short _InterlockedAnd16_np(short volatile* _Value, short _Mask);
		__int64 _InterlockedAnd64_np(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedAnd8_np(char volatile* _Value, char _Mask);
		long _InterlockedAnd_np(long volatile* _Value, long _Mask);
		unsigned char _interlockedbittestandset(long volatile*, long);
		unsigned char _InterlockedCompareExchange128_np(__int64 volatile* _Destination, __int64 _ExchangeHigh, __int64 _ExchangeLow, __int64* _ComparandResult);
		short _InterlockedCompareExchange16_np(short volatile* _Destination, short _Exchange, short _Comparand);
		__int64 _InterlockedCompareExchange64_np(__int64 volatile* _Destination, __int64 _Exchange, __int64 _Comparand);
		void* _InterlockedCompareExchangePointer(void* volatile* _Destination, void* _Exchange, void* _Comparand);
		void* _InterlockedCompareExchangePointer_np(void* volatile* _Destination, void* _Exchange, void* _Comparand);
		long _InterlockedCompareExchange_np(long volatile* _Destination, long _Exchange, long _Comparand);
		void* _InterlockedExchangePointer(void* volatile* _Target, void* _Value);
		short _InterlockedOr16_np(short volatile* _Value, short _Mask);
		__int64 _InterlockedOr64_np(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedOr8_np(char volatile* _Value, char _Mask);
		long _InterlockedOr_np(long volatile* _Value, long _Mask);
		short _InterlockedXor16_np(short volatile* _Value, short _Mask);
		__int64 _InterlockedXor64_np(__int64 volatile* _Value, __int64 _Mask);
		char _InterlockedXor8_np(char volatile* _Value, char _Mask);
		long _InterlockedXor_np(long volatile* _Value, long _Mask);
		void _ReadBarrier();
		void* _ReturnAddress();
		void _WriteBarrier();
		void __addgsbyte(unsigned long, unsigned char);
		void __addgsdword(unsigned long, unsigned long);
		void __addgsqword(unsigned long, unsigned __int64);
		void __addgsword(unsigned long, unsigned short);
		void __clts();
		void __code_seg(const char*);
		void __cpuid(int[4], int);
		void __cpuidex(int[4], int, int);
		void __debugbreak();
		__int64 __emul(int, int);
		unsigned __int64 __emulu(unsigned int, unsigned int);
		__declspec(noreturn) void __fastfail(unsigned int);
		void __faststorefence();
		unsigned int __getcallerseflags();
		void __halt();
		unsigned char __inbyte(unsigned short);
		void __inbytestring(unsigned short, unsigned char*, unsigned long);
		void __incgsbyte(unsigned long);
		void __incgsdword(unsigned long);
		void __incgsqword(unsigned long);
		void __incgsword(unsigned long);
		unsigned long __indword(unsigned short);
		void __indwordstring(unsigned short, unsigned long*, unsigned long);
		void __int2c();
		void __invlpg(void*);
		unsigned short __inword(unsigned short);
		void __inwordstring(unsigned short, unsigned short*, unsigned long);
		void __lidt(void*);
		unsigned __int64 __ll_lshift(unsigned __int64, int);
		__int64 __ll_rshift(__int64, int);
		void __movsb(unsigned char*, unsigned char const*, size_t);
		void __movsd(unsigned long*, unsigned long const*, size_t);
		void __movsq(unsigned long long*, unsigned long long const*, size_t);
		void __movsw(unsigned short*, unsigned short const*, size_t);
		__int64 __mulh(__int64, __int64);
		void __nop();
		void __nvreg_restore_fence();
		void __nvreg_save_fence();
		void __outbyte(unsigned short, unsigned char);
		void __outbytestring(unsigned short, unsigned char*, unsigned long);
		void __outdword(unsigned short, unsigned long);
		void __outdwordstring(unsigned short, unsigned long*, unsigned long);
		void __outword(unsigned short, unsigned short);
		void __outwordstring(unsigned short, unsigned short*, unsigned long);
		unsigned __int64 __rdtsc();
		unsigned __int64 __rdtscp(unsigned int*);
		unsigned __int64 __readcr0();
		unsigned __int64 __readcr2();
		unsigned __int64 __readcr3();
		unsigned __int64 __readcr4();
		unsigned __int64 __readcr8();
		unsigned __int64 __readdr(unsigned int);
		unsigned __int64 __readeflags();
		unsigned char __readgsbyte(unsigned long);
		unsigned long __readgsdword(unsigned long);
		unsigned __int64 __readgsqword(unsigned long);
		unsigned short __readgsword(unsigned long);
		unsigned __int64 __readmsr(unsigned long);
		void __readmsrlist(unsigned __int64, unsigned __int64*, unsigned __int64*);
		unsigned __int64 __readpmc(unsigned long);
		unsigned long __segmentlimit(unsigned long);
		void __sidt(void*);
		void __stosb(unsigned char*, unsigned char, size_t);
		void __stosd(unsigned long*, unsigned long, size_t);
		void __stosq(unsigned __int64*, unsigned __int64, size_t);
		void __stosw(unsigned short*, unsigned short, size_t);
		void __svm_clgi();
		void __svm_invlpga(void*, int);
		void __svm_skinit(int);
		void __svm_stgi();
		void __svm_vmload(size_t);
		void __svm_vmrun(size_t);
		void __svm_vmsave(size_t);
		void __ud2();
		unsigned __int64 __ull_rshift(unsigned __int64, int);
		unsigned __int64 __umulh(unsigned __int64, unsigned __int64);
		void __vmx_off();
		unsigned char __vmx_on(unsigned __int64*);
		unsigned char __vmx_vmclear(unsigned __int64*);
		unsigned char __vmx_vmlaunch();
		unsigned char __vmx_vmptrld(unsigned __int64*);
		void __vmx_vmptrst(unsigned __int64*);
		unsigned char __vmx_vmread(size_t, size_t*);
		unsigned char __vmx_vmresume();
		unsigned char __vmx_vmwrite(size_t, size_t);
		void __wbinvd();
		void __writecr0(unsigned __int64);
		void __writecr2(unsigned __int64);
		void __writecr3(unsigned __int64);
		void __writecr4(unsigned __int64);
		void __writecr8(unsigned __int64);
		void __writedr(unsigned int, unsigned __int64);
		void __writeeflags(unsigned __int64);
		void __writegsbyte(unsigned long, unsigned char);
		void __writegsdword(unsigned long, unsigned long);
		void __writegsqword(unsigned long, unsigned __int64);
		void __writegsword(unsigned long, unsigned short);
		void __writemsr(unsigned long, unsigned __int64);
		void __writemsrlist(unsigned __int64, unsigned __int64*, unsigned __int64*);
		void __writemsrns(unsigned long, unsigned __int64);
		unsigned char _bittest64(__int64 const*, __int64);
		unsigned char _bittestandcomplement(long*, long);
		unsigned char _bittestandcomplement64(__int64*, __int64);
		unsigned char _bittestandreset(long*, long);
		unsigned char _bittestandreset64(__int64*, __int64);
		unsigned char _bittestandset(long*, long);
		unsigned char _bittestandset64(__int64*, __int64);
		[[nodiscard]] unsigned __int64 _byteswap_uint64(unsigned __int64);
		[[nodiscard]] unsigned long _byteswap_ulong(unsigned long);
		[[nodiscard]] unsigned short _byteswap_ushort(unsigned short);
		void _disable();
		void _enable();
		unsigned char _interlockedbittestandreset(long volatile*, long);
		unsigned char _interlockedbittestandreset64(__int64 volatile*, __int64);
		unsigned char _interlockedbittestandset64(__int64 volatile*, __int64);
		[[nodiscard]] unsigned long _lrotl(unsigned long, int);
		[[nodiscard]] unsigned long _lrotr(unsigned long, int);
}