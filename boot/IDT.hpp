#pragma once
#include "stdtype.hpp"
struct IDT64
{
	u16 offsetLow;
	u16 selector;
	u8 ist;
	u8 typeAttr;
	u16 offsetMid;
	u32 offsetHigh;
	u32 zero;
	void setOffset(u64* offset);
	u64* getOffset();
};