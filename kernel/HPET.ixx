export module HPET;
import types;
import SDT;
import memory.PageTableManager;

export namespace ACPI
{
#pragma pack(1)
    struct HPET
    {
        enum Registers
        {
            GCID = 0x00,
            GCR = 0x10,
            GISR = 0x20,
            MCV = 0xF0,
        };
        SDT header;
        u8 hardware_rev_id;
        u8 comparator_count : 5;
        u8 counter_size : 1;
        u8 rsv0 : 1;
        u8 legacy_replacement : 1;
        u16 pci_vendor_id;
        u8 address_space_id;
        u8 register_bit_width;
        u8 register_bit_offset;
        u8 rsv1;
        u64 address;
        u8 hpet_number;
        u16 minimum_tick;
        u8 page_protection;
        void initialize()
        {
            memory::pageTableManager.mapMemory(reinterpret_cast<const void*>(address), reinterpret_cast<const void*>(address), memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE);
        }
        u64 read(u16 reg)
        {
            return *reinterpret_cast<u64*>(address + reg);
        }
        void write(u16 reg, u64 value)
        {
            *reinterpret_cast<u64*>(address + reg) = value;
        }
        void writeTimerN(u8 n, u64 value)
        {
            if (n > comparator_count) return;
            *reinterpret_cast<u64*>(address + 0x100 + 0x20 * n) = value;
        }
        void disable()
        {
            write(Registers::GCR, 0b10);
        }
        void enable()
        {
            write(Registers::GCR, 0b11);
        }
    } *hpet;
}