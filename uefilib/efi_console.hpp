#pragma once
#include "efi_types.hpp"
namespace efi
{
	struct SimpleTextInputInterface
	{
		Status(*reset) (SimpleTextInputInterface* simpleInputInterface, bool extendedVerification);
		Status(*readKeyStroke) (SimpleTextInputInterface* simpleInputInterface, InputKey* key);
		Event waitForKey;
	};
	using SimpleTextInputProtocol = SimpleTextInputInterface;

	struct SimpleTextOutputMode
	{
		int32_t maxMode;
		int32_t mode;
		int32_t attribute;
		int32_t cursorColumn;
		int32_t cursorRow;
		bool cursorVisible;
	};

	struct SimpleTextOutputInterface
	{
		Status(*reset) (SimpleTextOutputInterface* simpleTextOutputInterface, bool extendedVerification);
		Status(*outputString) (SimpleTextOutputInterface* simpleTextOutputInterface, const char16_t* String);
		Status(*testString) (SimpleTextOutputInterface* simpleTextOutputInterface, const char16_t* string);
		Status(*queryMode) (SimpleTextOutputInterface* simpleTextOutputInterface, uint64_t modeNumber, uint64_t* columns, uint64_t* rows);
		Status(*setMode) (SimpleTextOutputInterface* simpleTextOutputInterface, uint64_t modeNumber);
		Status(*setAttribute) (SimpleTextOutputInterface* simpleTextOutputInterface, uint64_t attribute);
		Status(*clearScreen) (SimpleTextOutputInterface* simpleTextOutputInterface);
		Status(*setCursorPosition) (SimpleTextOutputInterface* simpleTextOutputInterface, uint64_t column, uint64_t row);
		Status(*enableCursor) (SimpleTextOutputInterface* simpleTextOutputInterface, bool enable);
		SimpleTextOutputMode* Mode;
	};
	using SimpleTextOutputProtocol = SimpleTextOutputInterface;
}