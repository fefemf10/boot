#pragma once
#include <stdint.h>
namespace efi
{
	struct GUID
	{
		uint32_t data1;
		uint16_t data2;
		uint16_t data3;
		uint8_t data4[8];
		constexpr inline bool operator==(const GUID& other) const
		{
			return reinterpret_cast<const uint64_t*>(this)[0] == reinterpret_cast<const uint64_t*>(&other)[0]
				&& reinterpret_cast<const uint64_t*>(this)[1] == reinterpret_cast<const uint64_t*>(&other)[1];
		}
	};
}