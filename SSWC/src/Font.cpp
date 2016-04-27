#include "Font.h"


Font::Font()
{

}

Font::Font(HDC hDC, int height, int width, float angleOfEscapement, float orientationAngle, int fontWeight,
	bool italic, bool underline, bool strikeout, int charsetIdentifyer, int outputQuality,
	std::string fontName)
{
	base = glGenLists(/*96*/256);
	font = CreateFont(height, width, angleOfEscapement, orientationAngle, fontWeight, italic, underline, strikeout, charsetIdentifyer,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, outputQuality, FF_DONTCARE | DEFAULT_PITCH, fontName.c_str());

	oldfont = (HFONT)SelectObject(hDC, font);			// Selects The Font We Want
	wglUseFontBitmaps(hDC, /*32*/0, /*96*/255, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}


Font::~Font()
{
}

void Font::glPrint(const char* fmt, ...)
{
	char text[256];										// Holds Our String
	va_list ap;											// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base/*-32*/);								// Sets The Base Character to 32

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}
