#pragma once
#include <windows.h>                      
#include <math.h>                     
#include <stdio.h>                       
#include <stdarg.h>
#include <gl/gl.h>
#include <gl/glu.h>	
#include <string>

class Font
{
private:
	GLuint base;
	HFONT font;
	HFONT oldfont;

public:
	Font();

	Font(HDC hDCint, int height = -24, int width = -10, float angleOfEscapement = 0, float orientationAngle = 0, int fontWeight = FW_BOLD,
		bool italic = false, bool underline = false, bool strikeout = false, int charsetIdentifyer = ANSI_CHARSET,
		int outputQuality = ANTIALIASED_QUALITY, std::string fontName = "Courier New");

	~Font();

	void KillFont() { glDeleteLists(base, 96); }
	void glPrint(const char* fmt, ...);
};

/*HDC hDCint, int height = -24, int width = 0, float angleOfEscapement = 0, float orientationAngle = 0, int fontWeight = FW_BOLD,
bool italic = false, bool underline = false, bool strikeout = false, int charsetIdentifyer = ANSI_CHARSET,
int outputQuality = ANTIALIASED_QUALITY, std::string fontName = "Courier New"

HDC hDC, int height, int width, float angleOfEscapement, float orientationAngle, int fontWeight,
bool italic, bool underline, bool strikeout, int charsetIdentifyer, int outputQuality,
std::string fontName*/