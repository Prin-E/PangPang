#pragma once

// C++ 헤더
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <mutex>

// C 헤더
#include <cwchar>
#include <cstring>

#include <boost/signals2.hpp>

// 플랫폼 종속 헤더
#if defined(WIN32)
#include <windows.h>
#include <memory>
#include <GL/glew.h>
#include <GL/wglew.h>
#elif defined(_LINUX)
#include <memory>
#include <GL/glew.h>
#elif defined(__APPLE__)
#include <tr1/memory>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

namespace std {
	using namespace std::tr1;
};
#endif

