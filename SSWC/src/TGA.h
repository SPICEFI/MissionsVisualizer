#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

//class to load tga files into openGL
class TGA
{
protected:
	GLuint textureHandle;
public:
	TGA(const char* imagePath);
	~TGA();

	GLuint getTextureHandle();
};

