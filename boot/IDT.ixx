export module IDT;
import types;
import teletype;
import PIC;
export namespace IDT
{
	enum Gate : u8
	{
		INTERRUPT = 0xE,
		TRAP = 0xF
	};
#pragma pack(1)
	struct IDTR
	{
		u16 size;
		u64 offset;
	};
	struct IDT
	{
		u16 offsetLow;
		u16 selector;
		u8 ist;
		u8 typeAttr;
		u16 offsetMid;
		u32 offsetHigh;
		u32 zero;
		void setOffset(void (*function)())
		{
			offsetLow = static_cast<u16>(reinterpret_cast<u64>(function) & 0x000000000000ffff);
			offsetMid = static_cast<u16>((reinterpret_cast<u64>(function) & 0x00000000ffff0000) >> 16u);
			offsetHigh = static_cast<u32>((reinterpret_cast<u64>(function) & 0xffffffff00000000) >> 32u);
		}
		u64* getOffset()
		{
			u64 offset{};
			offset |= static_cast<u64>(offsetLow);
			offset |= static_cast<u64>(offsetMid) << 16u;
			offset |= static_cast<u64>(offsetHigh) << 32u;
			return reinterpret_cast<u64*>(offset);
		}
	};
	IDT idt[256]{};
	IDTR idtr{};
	extern "C" void loadIDTR(const IDTR* idtr);
	void set(size_t index, void(*function)())
	{
		idt[index].setOffset(function);
		idt[index].selector = 0x08;
		idt[index].typeAttr = 0x80 | (index < 32 ? Gate::TRAP : Gate::INTERRUPT);
	}
	void initialize()
	{
		idtr.size = 4095;
		idtr.offset = reinterpret_cast<u64>(idt);
		PIC::initialize();
		loadIDTR(&idtr);
	}
}