export module Font;
import types;
export
{
	struct PSF1Font
	{
		u8 magic[2];
		u8 mode;
		u8 charSize;
		u8 glyphBuffer[1];
	} *font;
	u64 fontSize;
}