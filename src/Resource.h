#pragma once

#if defined(_WIN32)
#include "win32/ResourceWin32.h"
#define Resource ResourceWin32
#elif defined(__APPLE__)
#include "ResourceCocoa.h"
#define Resource ResourceCocoa
#elif defined(_LINUX)
#include "posix/ResourcePosix.h"
#define Resource ResourcePosix
#endif

