#pragma once

#include <queue>
#include <map>
#include <string>

#include <Windows.h>
#include <GL/glew.h>

#include "../Types.h"
#include "../Event.h"

namespace PangPang
{
class Renderer;

class WindowWin32 : public Event::Emitter<Event::Mouse>, public Event::Emitter<Event::Keyboard>
{
	friend class Renderer;
public:
	WindowWin32(const wchar_t* title, Size size);
	~WindowWin32(void);
public:
	void StartMessageLoop();
	void StopMessageLoop();

	Size GetContentSize();
	double FPS();
	double DeltaTime();
	double GetElapsedTime();
protected:
	virtual void UpdateGame(double deltaTime)=0;
private:
	static LRESULT CALLBACK WndProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void AddEventToKeyboardEventQueueForVK(unsigned int vk, UINT msg);
	void AddEventToMouseEventQueue(UINT msg, WPARAM wParam, LPARAM lParam);

	void InitContext();
	void InitOpenGLView();

	void SwapBuffers();

	void Clip(bool clip);
private:
	HINSTANCE hInstance;
	HWND hWnd;
	HDC hDC;
	HGLRC hGLRC;

	bool isRunning;
	std::wstring windowTitle;

	Rect windowRect;
	Size contentSize;

	LARGE_INTEGER queryFrequency, queryCounter;
	LARGE_INTEGER prevElapsedTime, elapsedTime;
	double deltaTime;
	double fps;

	bool clipped;
};
}

