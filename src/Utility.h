#pragma once
#include "Common.h"

void GetOpenGLVersion(unsigned int &major, unsigned int &minor) {
	const char *version = (const char *)glGetString(GL_VERSION);
	if(version)
		sscanf(version, "%d.%d", &major, &minor);
}

