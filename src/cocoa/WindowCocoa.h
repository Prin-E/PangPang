#pragma once

#include "Common.h"

#include "Types.h"
#include "Event.h"

namespace PangPang {
class Renderer;
class WindowCocoa : public Event::Emitter<Event::Mouse>, public Event::Emitter<Event::Keyboard>
{
	friend class Renderer;
public:
	WindowCocoa(const wchar_t* title, Size size);
	~WindowCocoa(void);
public:
	void StartMessageLoop();
	void StopMessageLoop();
	
	double FPS();
	double DeltaTime();
	double GetElapsedTime();
protected:
	virtual void UpdateGame(double deltaTime)=0;
private:
	
	void AddEventToKeyboardEventQueueForVK(void *event); // NSEvent
	
	void InitContext();
	void InitOpenGLView();
	
	void SwapBuffers();
private:
	void *window;			// NSWindow
	void *openGLView;		// NSOpenGLView
	void *openGLContext;	// NSOpenGLContext
	void *cglContext;		// CGLContextObj
	
	void *timerDate;		// NSDate;
	
	bool isRunning;
	std::wstring windowTitle;
	
	Rect windowRect;
	
	double prevElapsedTime, elapsedTime;
	double deltaTime;
	double fps;
};
}

