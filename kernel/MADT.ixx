export module MADT;
import types;
import SDT;
export namespace ACPI
{
	const char8_t* Types[]
	{
		u8"PLAPIC",
		u8"IOAPIC",
		u8"ISO",
		u8"NMI",
		u8"LAPICNMI",
		u8"LAPICO",
		u8"IOSAPIC",
		u8"LSAPIC",
		u8"PIS",
		u8"PLx2APIC",
		u8"Lx2APICNMI"
	};
	enum class Type : u8
	{
		PLAPIC,
		IOAPIC,
		ISO,
		NMI,
		LAPICNMI,
		LAPICO,
		IOSAPIC,
		LSAPIC,
		PIS,
		PLx2APIC,
		Lx2APICNMI
	};
	/// <summary>
	/// Processor Local APIC
	/// </summary>
	struct PLAPIC
	{
		Type type;
		u8 length;
		u8 processorId;
		u8 apicId;
		u32 flags;
		constexpr bool isValid()
		{
			return length == sizeof(PLAPIC);
		}
		constexpr bool isOnline()
		{
			return flags == 1; //check logical core is online
		}
	};
	/// <summary>
	/// I/O APIC
	/// </summary>
	struct IOAPIC
	{
		Type type;
		u8 length;
		u8 id;
		u8 rsv;
		u32 address;
		u32 GSIBase;
		constexpr bool isValid()
		{
			return length == sizeof(IOAPIC);
		}
	};
#pragma pack(1)
	/// <summary>
	/// Local APIC Address Override
	/// </summary>
	struct LAPICO
	{
		Type type;
		u8 length;
		u16 rsv;
		void* lapic;
		constexpr bool isValid()
		{
			return length == sizeof(LAPICO);
		}
	};
	/// <summary>
	/// Multiple APIC Description Table
	/// </summary>
	struct MADT
	{
		SDT header;
		u32 lapic;
		u32 flags;
		struct Entry
		{
			Type type;
			u8 length;
		} entries[1];
		constexpr u64 sizeEntries() const
		{
			return (header.length - sizeof(SDT) - sizeof(lapic) - sizeof(flags)) / sizeof(Entry);
		}
	} *madt;
}