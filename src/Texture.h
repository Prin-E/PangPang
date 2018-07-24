#pragma once
#include <map>
#include <string>
#include "Types.h"

namespace PangPang
{
enum ImageFormat {
	NoneImageFormat = 0,
	BlackAndWhiteFormat,
	BlackWhiteAndAlphaFormat,
	RGBFormat,
	BGRFormat,
	RGBAFormat,
	BGRAFormat
};

const unsigned int nullImageSize = 128;
const unsigned int nullbpp = 4;

class Texture
{
	// member function
public:
	Texture(void);
	Texture(const char *path);
	Texture(unsigned char *image, Size size, int bpp);
	~Texture(void);

	void SetImageInFile(const char *path);
	void SetImageInMemory(unsigned char *image, Size size, int bpp);

	void BindTexture();

	Size ImageSize() {
		return imageRect.size;
	}

	unsigned int ImageFormat() {
		return imageFormat;
	}

	void *Lock();
	void Unlock();

protected:
private:
	void InitNullTexture();
	// member variable
public:
protected:
private:
	unsigned int textureID;

	// OpenGL 2.1+
	unsigned int bufferID;
	// OpenGL 1.1
	unsigned char *imageData;

	Rect imageRect;

	unsigned int imageFormat;

	static unsigned char nullImage[nullImageSize * nullImageSize * nullbpp];
};
}

