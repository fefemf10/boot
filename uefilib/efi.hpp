#pragma once
#include "efi_types.hpp"


struct SIMPLE_INPUT_INTERFACE;
using EFI_INPUT_RESET = EFI_STATUS(*) (SIMPLE_INPUT_INTERFACE* This, BOOLEAN ExtendedVerification);
using EFI_INPUT_READ_KEY = EFI_STATUS(*) (SIMPLE_INPUT_INTERFACE* This, EFI_INPUT_KEY* Key);

struct SIMPLE_INPUT_INTERFACE
{
	EFI_INPUT_RESET                     Reset;
	EFI_INPUT_READ_KEY                  ReadKeyStroke;
	EFI_EVENT                           WaitForKey;
};

using EFI_SIMPLE_TEXT_IN_PROTOCOL = SIMPLE_INPUT_INTERFACE;
struct SIMPLE_TEXT_OUTPUT_INTERFACE;

using EFI_TEXT_CLEAR_SCREEN = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This);
using EFI_TEXT_SET_CURSOR_POSITION = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN Column, UINTN Row);
using EFI_TEXT_ENABLE_CURSOR = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, BOOLEAN Enable);

struct SIMPLE_TEXT_OUTPUT_MODE
{
	INT32                           MaxMode;
	// current settings
	INT32                           Mode;
	INT32                           Attribute;
	INT32                           CursorColumn;
	INT32                           CursorRow;
	BOOLEAN                         CursorVisible;
};

using EFI_TEXT_RESET = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, BOOLEAN ExtendedVerification);
using EFI_TEXT_OUTPUT_STRING = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, const CHAR16* String);
using EFI_TEXT_TEST_STRING = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, CHAR16* String);
using EFI_TEXT_QUERY_MODE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN ModeNumber, UINTN* Columns, UINTN* Rows);
using EFI_TEXT_SET_MODE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN ModeNumber);
using EFI_TEXT_SET_ATTRIBUTE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN Attribute);


struct SIMPLE_TEXT_OUTPUT_INTERFACE
{
	EFI_TEXT_RESET                  Reset;

	EFI_TEXT_OUTPUT_STRING          OutputString;
	EFI_TEXT_TEST_STRING            TestString;

	EFI_TEXT_QUERY_MODE             QueryMode;
	EFI_TEXT_SET_MODE               SetMode;
	EFI_TEXT_SET_ATTRIBUTE          SetAttribute;

	EFI_TEXT_CLEAR_SCREEN           ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR          EnableCursor;

	// Current mode
	SIMPLE_TEXT_OUTPUT_MODE* Mode;
};
using EFI_SIMPLE_TEXT_OUT_PROTOCOL = SIMPLE_TEXT_OUTPUT_INTERFACE;