#pragma once
#include "efi_types.hpp"
namespace efi
{
	struct GraphicsOutputBltPixel
	{
		uint8_t Blue;
		uint8_t Green;
		uint8_t Red;
		uint8_t Reserved;
	};

	union GraphicsOutputBltPixelUnion
	{
		GraphicsOutputBltPixel Pixel;
		uint32_t Raw;
	};

	enum class GraphicsOutputBltOperation
	{
		BLT_VIDEO_FILL,
		BLT_VIDEOTO_BLT_BUFFER,
		BLT_BUFFER_TO_VIDEO,
		BLT_VIDEO_TO_VIDEO,
		GRAPHICS_OUTPUT_BLT_OPERATION_MAX
	};

	enum class GraphicsPixelFormat : uint32_t
	{
		PIXEL_RED_GREEN_BLUE_RESERVED_8BIT_PER_COLOR,
		PIXEL_BLUE_GREEN_RED_RESERVED_8BIT_PER_COLOR,
		PIXEL_BIT_MASK,
		PIXEL_BLT_ONLY,
		PIXEL_FORMAT_MAX
	};

	struct PixelBitmask
	{
		uint32_t redMask;
		uint32_t greenMask;
		uint32_t blueMask;
		uint32_t reservedMask;
	};

	struct GraphicsOutputModeInformation
	{
		uint32_t version;
		uint32_t horizontalResolution;
		uint32_t verticalResolution;
		GraphicsPixelFormat pixelFormat;
		PixelBitmask pixelInformation;
		uint32_t pixelsPerScanLine;
	} ;

	struct GraphicsOutputProtocolMode
	{
		uint32_t maxMode;
		uint32_t mode;
		GraphicsOutputModeInformation* info;
		uint64_t sizeOfInfo;
		PhysicalAddress frameBufferBase;
		uint64_t frameBufferSize;
	};

	struct GraphicsOutputProtocol
	{
		Status (*queryMode) (GraphicsOutputProtocol* graphicsOutputProtocol, uint32_t modeNumber, uint64_t* sizeOfInfo, GraphicsOutputModeInformation** info);
		Status (*setMode) (GraphicsOutputProtocol* graphicsOutputProtocol, uint32_t modeNumber);
		Status (*blt) (GraphicsOutputProtocol* graphicsOutputProtocol, GraphicsOutputBltPixel* graphicsOutputBltPixel, GraphicsOutputBltOperation bltOperation, uint64_t sourceX, uint64_t sourceY, uint64_t destinationX, uint64_t destinationY, uint64_t width, uint64_t height, uint64_t delta);
		GraphicsOutputProtocolMode* mode;
	};
}