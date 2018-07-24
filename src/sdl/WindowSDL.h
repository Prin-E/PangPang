#pragma once

#include <queue>
#include <map>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <SDL/SDL.h>

#include "../Types.h"
#include "../Event.h"

namespace PangPang
{
class Renderer;

class WindowSDL : public Event::Emitter<Event::Mouse>, public Event::Emitter<Event::Keyboard>
{
	friend class Renderer;
public:
	WindowSDL(const wchar_t* title, Size size);
	~WindowSDL(void);
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
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void eventLoop();

	void AddEventToKeyboardEventQueueForVK(unsigned int vk, Uint8 evtype);
	void AddEventToMouseEventQueue(SDL_Event *ev);

	void InitOpenGLView();

	void SwapBuffers();
	
	void initTimer();
	void endTimer();
	double getElapsedTime() const;
private:
	SDL_Surface *screen;

	bool isRunning;
	std::wstring windowTitle;

	Rect windowRect;
	Size contentSize;

	//LARGE_INTEGER queryFrequency, queryCounter;
	//LARGE_INTEGER prevElapsedTime, elapsedTime;
	time_t startTime;
	double elapsedTime, prevElapsedTime; // in miliseconds
	double deltaTime;
	double fps;
};

} /* END OF namespace PangPang */




