#pragma once

#include "Window.h"
#include "Types.h"
#include "State.h"

namespace PangPang
{
class GameApplication : protected Window
{

public:
	GameApplication(int argc, char *argv[], const wchar_t* title, Size size);

	virtual ~GameApplication(void);

	using Window::FPS;
	using Window::GetElapsedTime;

	void Run();
	void Stop();

protected:
	Rect windowRect;
	Size gameResolution;
};
}

