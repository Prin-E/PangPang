#pragma once

#if defined(_WIN32)
#include "win32/WindowWin32.h"
#define Window WindowWin32
#elif defined(__APPLE__)
#include "WindowCocoa.h"
#define Window WindowCocoa
#elif defined(USE_SDL)
#include "sdl/WindowSDL.h"
#define Window WindowSDL
#endif

