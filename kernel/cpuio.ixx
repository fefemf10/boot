export module cpuio;
import types;
import intrinsic1;
export namespace cpuio
{

	struct regs
	{
		u64 rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
		u64 interruptCode, errorCode;
		u64 rip, cs, rflags, useresp, ss;
	};

#pragma pack(1)
	struct Features
	{
		u32 eax;
		u32 ebx;

		u64 SSE3 : 1;
		u64 PCLMUL : 1;
		u64 DTES64 : 1;
		u64 MONITOR : 1;
		u64 DS_CPL : 1;
		u64 VMX : 1;
		u64 SMX : 1;
		u64 EST : 1;
		u64 TM2 : 1;
		u64 SSSE3 : 1;
		u64 CID : 1;
		u64 SDBG : 1;
		u64 FMA : 1;
		u64 CX16 : 1;
		u64 XTPR : 1;
		u64 PDCM : 1;
		u64 rsv1 : 1;
		u64 PCID : 1;
		u64 DCA : 1;
		u64 SSE4_1 : 1;
		u64 SSE4_2 : 1;
		u64 X2APIC : 1;
		u64 MOVBE : 1;
		u64 POPCNT : 1;
		u64 ETSC : 1;
		u64 AES : 1;
		u64 XSAVE : 1;
		u64 OSXSAVE : 1;
		u64 AVX : 1;
		u64 F16C : 1;
		u64 RDRAND : 1;
		u64 HYPERVISOR : 1;
		
		u64 FPU : 1;
		u64 VME : 1;
		u64 DE : 1;
		u64 PSE : 1;
		u64 DTSC : 1;
		u64 MSR : 1;
		u64 PAE : 1;
		u64 MCE : 1;
		u64 CX8 : 1;
		u64 APIC : 1;
		u64 rsv2 : 1;
		u64 SEP : 1;
		u64 MTRR : 1;
		u64 PGE : 1;
		u64 MCA : 1;
		u64 CMOV : 1;
		u64 PAT : 1;
		u64 PSE36 : 1;
		u64 PSN : 1;
		u64 CLFLUSH : 1;
		u64 rsv3 : 1;
		u64 DS : 1;
		u64 ACPI : 1;
		u64 MMX : 1;
		u64 FXSR : 1;
		u64 SSE : 1;
		u64 SSE2 : 1;
		u64 SS : 1;
		u64 HTT : 1;
		u64 TM : 1;
		u64 IA64 : 1;
		u64 PBE : 1;
	} features;
	u64 busFrequencyHz;
	u64 coreFrequencyMHz;
	void enableSSE();
	void enableAVX();
	void loadGDT(void* gdtDesciptor);
	void loadIDTR(void* idtrDesciptor);
	void getCPUFeatures(Features& features)
	{
		__cpuid(reinterpret_cast<int*>(&features), 0x1);
	}
	bool isEnabledInterrupt();
	void enableNMI()
	{
		__outbyte(0x70, __inbyte(0x70) & 0x7F);
		__inbyte(0x71);
	}
	void disableNMI()
	{
		__outbyte(0x70, __inbyte(0x70) | 0x80);
		__inbyte(0x71);
	}
}