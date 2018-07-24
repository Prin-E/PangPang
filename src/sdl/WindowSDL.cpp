#include "WindowSDL.h"
#include <stdio.h>

#ifdef _LINUX
#include <sys/time.h>
#endif


namespace PangPang
{

// 초기화 메서드
WindowSDL::WindowSDL(const wchar_t* title, Size size)
	:	windowTitle(title)
{
	int ret;
	
	ret = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	if(ret < 0)
		::abort();
	
	windowRect.size.width = 800;
	windowRect.size.height = 600;
	contentSize = Size(size.width, size.height);
	
	this->screen = SDL_SetVideoMode(windowRect.size.width, windowRect.size.height, 0, SDL_OPENGL);
	//SDL_WM_SetCaption(title);
	
	glewInit(); // GLEW
	InitOpenGLView(); // OpenGL 뷰 초기화
	
	initTimer();
}

void WindowSDL::InitOpenGLView()
{
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, windowRect.size.width, windowRect.size.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (double)windowRect.size.width, 0, (double)windowRect.size.height, 100.0, -100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WindowSDL::SwapBuffers()
{
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowSDL::initTimer()
{
	this->startTime = 0;
	this->startTime = this->getElapsedTime();
}
void WindowSDL::endTimer()
{
}

double WindowSDL::getElapsedTime() const
{
#ifdef _LINUX
	struct timeval tv;
	::gettimeofday(&tv, NULL);
	double time = (tv.tv_sec - this->startTime) + ((double)tv.tv_usec / 1000000);
	return time;
#else
	// very low resolution, unusable
	return SDL_GetTicks() / 1000;
# warning WindowSDL::getElapsedTime() is not implemented. Using default low-resolution timer.
#endif
}


// 윈도우 프로시저
void WindowSDL::eventLoop()
{
	SDL_Event ev;
	for(int i = 0; i < 1000 && SDL_PollEvent(&ev); i++)
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if(ev.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			}
			
			AddEventToKeyboardEventQueueForVK((unsigned int)ev.key.keysym.sym, ev.type);
			break;
		
		case SDL_MOUSEMOTION:
			// ev.motion.
			AddEventToMouseEventQueue(&ev);
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			// ev.button.
			if(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF)
			{
				SDL_WM_GrabInput(SDL_GRAB_ON);
			}
			
			/* fallthrough */
		case SDL_MOUSEBUTTONUP:
			AddEventToMouseEventQueue(&ev);
			break;
		
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}
}

// 메시지 루프 시작/종료
void WindowSDL::StartMessageLoop()
{
	isRunning = true;
	while(isRunning) {
		eventLoop();
		
		elapsedTime = getElapsedTime();
		deltaTime = (double)(elapsedTime - prevElapsedTime);

		// 이벤트 분배
		Event::Emitter<Event::Keyboard>::Emit();
		Event::Emitter<Event::Mouse>::Emit();

		// 게임 업데이트
		UpdateGame(deltaTime);

		// FPS 계산
		prevElapsedTime = elapsedTime;
		fps = 1.0 / deltaTime;
	}
}

void WindowSDL::AddEventToKeyboardEventQueueForVK(unsigned int vk, Uint8 msg)
{
	Event::Keyboard event;

	static uint32 modifierFlags = 0;

	switch(msg) {
	case SDL_KEYDOWN:
		event.type = Event::Keyboard::Down;

		if(vk == VKShift)
			modifierFlags = modifierFlags | ShiftFlags;
		else if(vk == VKControl)
			modifierFlags = modifierFlags | ControlFlags;
		break;
	case SDL_KEYUP:
		event.type = Event::Keyboard::Up;
		
		if(vk == VKShift)
			modifierFlags = modifierFlags ^ ShiftFlags;
		else if(vk == VKControl)
			modifierFlags = modifierFlags ^ ControlFlags;
		break;
	}

	event.keyCode = vk;
	event.flags = modifierFlags;

	Event::Emitter<Event::Keyboard>::list.push_back(event);
}


void WindowSDL::AddEventToMouseEventQueue(SDL_Event *ev) 
{
	Event::Mouse event;

	// for click count
	static unsigned int clickCount = 0;
	static double prevClickedTime = this->GetElapsedTime();
	static uint32 prevMouseType = Event::Mouse::None;

	double clickedTime = this->GetElapsedTime();
	
	int x, y;
	if(ev->type == SDL_MOUSEMOTION)
	{
		x = ev->motion.x;
		y = ev->motion.y;
	}
	else
	{
		x = ev->button.x;
		y = ev->button.y;
	}
	
	Point locationInWindow;
	locationInWindow.x = x;
	locationInWindow.y = contentSize.height - y;

	switch(ev->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		switch(ev->button.button)
		{
		case 1:
			event.type = Event::Mouse::LeftDown;
			if(event.type == prevMouseType && (clickedTime-prevClickedTime) <= 0.250)
				clickCount++;
			else
				clickCount = 1;

			event.clickCount = clickCount;
			prevMouseType = event.type;
		
			break;
		case 2:
			// 추후 지원 예정
			break;
		case 3:
			event.type = Event::Mouse::RightDown;
			if(event.type == prevMouseType && (clickedTime-prevClickedTime) <= 0.250)
				clickCount++;
			else
				clickCount = 1;

			event.clickCount = clickCount;
			prevMouseType = event.type;

			break;
		}
		break;
	
	case SDL_MOUSEBUTTONUP:
		switch(ev->button.button)
		{
		case 1:
			event.type = Event::Mouse::LeftUp;
			event.clickCount = 1;

			break;
		case 2:
			// 추후 지원 예정
			break;
		case 3:
			event.type = Event::Mouse::RightUp;
			event.clickCount = 1;
			
			break;
		}
		break;
	
	case SDL_MOUSEMOTION:
		if(ev->motion.state == 1)
			event.type = Event::Mouse::LeftDragged;
		else if(ev->motion.state == 3)
			event.type = Event::Mouse::RightDragged;
		else {
			event.type = Event::Mouse::Move;
			prevMouseType = Event::Mouse::Move;
		}
		clickCount = 0;
		break;
	}

	event.locationInWindow = locationInWindow;

	Event::Emitter<Event::Mouse>::list.push_back(event);

	prevClickedTime = clickedTime;
}

void WindowSDL::StopMessageLoop()
{
	isRunning = false;
}


double WindowSDL::GetElapsedTime()
{
	double t = elapsedTime / 1000;
	return t;
}

double WindowSDL::FPS()
{
	return fps;
}

double WindowSDL::DeltaTime()
{
	return deltaTime;
}

Size WindowSDL::GetContentSize()
{
	return contentSize;
}

WindowSDL::~WindowSDL()
{
	endTimer();
	SDL_Quit();
}


}




