#pragma once

#if defined WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <stdint.h>
#elif defined(_LINUX)
#include <inttypes.h>
#endif
#if defined(USE_SDL)
# include <SDL/SDL.h>
#endif

namespace PangPang
{
#if defined WIN32
typedef int int32;
typedef unsigned int uint;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef short int16;
typedef unsigned short uint16;
typedef float float32;
typedef double float64;
typedef unsigned char uchar;
#elif defined(__APPLE__)
typedef int int32;
typedef uint32_t uint;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef int16_t int16;
typedef unsigned short uint16;
typedef float float32;
typedef double float64;
typedef unsigned char uchar;
#elif defined(_LINUX)
typedef int32_t int32;
typedef uint32_t uint;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef int16_t int16;
typedef uint16_t uint16;
typedef float float32;
typedef double float64;
typedef unsigned char uchar;
#endif

struct Point {
	int x, y;

	Point(int x_, int y_) : x(x_), y(y_) {}
	Point() : x(0), y(0) {}

	float operator-(const Point& rhs)
	{
		return sqrt((float)((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y)));
	}
};

struct Size {
	Size(int width_, int height_) : width(width_), height(height_) {}
	Size() : width(0), height(0) {}

	int width, height;
};

struct Rect {
	Rect(int x, int y, int w, int h) : origin(x, y), size(w, h) {}
	Rect() : origin(0, 0), size(0, 0) {};

	Point origin;
	Size size;
};

struct Oval {
	Oval(Point theCenter, int rW, int rH) : center(theCenter), radiusW(rW), radiusH(rH) {}

	Point center;
	int radiusW;
	int radiusH;
};

struct Circle {
	Circle(Point theCenter, int theRadius) : center(theCenter), radius(theRadius) {}

	Point center;
	int radius;
};

struct Range {
	Range(uint32 theOrigin, uint32 theLength) : origin(theOrigin), length(theLength) {}

	uint32 origin;
	uint32 length;
};

struct Color {
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float rValue, float gValue, float bValue, float aValue)
		: r(rValue), g(gValue), b(bValue), a(aValue) {}

	float r, g, b, a;
};

enum VirtualKey {
#if defined WIN32
	VKTab = VK_TAB,
	VKShift = VK_SHIFT,
	VKControl = VK_CONTROL,
	VKAlt = VK_MENU,
	VKOption = VKAlt,
	VKEscape = VK_ESCAPE,
	VKSpace = VK_SPACE,
	VKLeft = VK_LEFT,
	VKRight = VK_RIGHT,
	VKUp = VK_UP,
	VKDown = VK_DOWN
#elif defined(__APPLE__)
	VKTab = 48,
	VKShift = 56,
	VKControl = 59,
	VKAlt = 58,
	VKOption = VKAlt,
	VKEscape = 53,
	VKSpace = 49,
	VKLeft = 123,
	VKRight = 124,
	VKUp = 126,
	VKDown = 125
#elif defined(USE_SDL)
	VKTab = SDLK_TAB,
	VKShift = SDLK_LSHIFT,
	VKControl = SDLK_LCTRL,
	VKAlt = SDLK_MENU,
	VKOption = VKAlt,
	VKEscape = SDLK_ESCAPE,
	VKSpace = SDLK_SPACE,
	VKLeft = SDLK_LEFT,
	VKRight = SDLK_RIGHT,
	VKUp = SDLK_UP,
	VKDown = SDLK_DOWN, 
#endif
};

enum ModifierFlags {
	ShiftFlags = 1,
	ControlFlags = 1 << 1,
	AltFlags = 1 << 2,
	OptionFlags = 1 << 3
};

enum TGAImageType {
	TGAImageTypeNone = 0,
	TGAImageTypeColorIndex = 1,
	TGAImageTypeTrueColor = 2,
	TGAImageTypeGrayscale = 3,
	TGAImageTypeColorIndexRLE = 9,
	TGAImageTypeTrueColorRLE = 10,
	TGAImageTypeGrayscaleRLE = 11
};

//#pragma pack(push)
#pragma pack(1)
struct TGAFileHeader {
	unsigned char IDLength;
	unsigned char colorMapType;
	unsigned char imageType;
	unsigned short colorMapStart;
	unsigned short colorMapLength;
	unsigned char colorMapDepth;
	unsigned short offsetX;
	unsigned short offsetY;
	unsigned short width;
	unsigned short height;
	unsigned char pixelDepth;
	unsigned char imageDescriptor;

	TGAFileHeader() : IDLength(0), colorMapType(0), imageType(0), colorMapStart(0),
		colorMapLength(0), colorMapDepth(0), offsetX(0), offsetY(0),
		width(0), height(0), pixelDepth(0), imageDescriptor(0)
	{
		// none
	}
};
#pragma pack()
//#pragma pack(pop)
}

