export module Font;
import types;
export
{
	struct PSF1Header
	{
		u8 magic[2];
		u8 mode;
		u8 charSize;
	};
	struct PSF1Font
	{
		PSF1Header* psf1Header;
		void* glyphBuffer;
	} *font;
}