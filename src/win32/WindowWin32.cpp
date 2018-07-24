#include "Common.h"
#include "WindowWin32.h"

namespace PangPang
{
// 초기화 메서드
WindowWin32::WindowWin32(const wchar_t* title, Size size)
	:	windowTitle(title)
{
	hInstance = (HINSTANCE)GetModuleHandle(NULL);
	WNDCLASSEX wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT wndRect;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProcThunk;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)windowTitle.c_str();
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	// 창 모드, 800x600 크기
	dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	wndRect.left = 0;
	wndRect.right = size.width;
	wndRect.top = 0;
	wndRect.bottom = size.height;
	AdjustWindowRectEx(&wndRect, dwStyle, false, dwExStyle);

	windowRect = Rect(100, 100, wndRect.right-wndRect.left, wndRect.bottom-wndRect.top);
	contentSize = Size(size.width, size.height);
	hWnd = CreateWindowEx(dwExStyle, (LPCWSTR)windowTitle.c_str(), (LPCWSTR)windowTitle.c_str(), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.size.width, windowRect.size.height, NULL, NULL, hInstance, this);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	// 컨텍스트 초기화
	InitContext();

	// QueryPeformance 설정
	QueryPerformanceFrequency(&queryFrequency);
	//prevElapsedTime.QuadPart = 0;
	QueryPerformanceCounter(&prevElapsedTime);
}

// 컨텍스트 초기화
void WindowWin32::InitContext() {
	hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof pfd);
	pfd.nSize = sizeof pfd;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 8;

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);

	glewInit(); // GLEW

	InitOpenGLView(); // OpenGL 뷰 초기화
}

void WindowWin32::InitOpenGLView() {
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, windowRect.size.width, windowRect.size.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (double)windowRect.size.width, 0, (double)windowRect.size.height, 100.0, -100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WindowWin32::SwapBuffers()
{
	::SwapBuffers(hDC);
	glClear(GL_COLOR_BUFFER_BIT);
}

// static 윈도우 프로시저
LRESULT WindowWin32::WndProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WindowWin32 *window = NULL;

	if(msg == WM_CREATE) {
		window = (WindowWin32 *)((LPCREATESTRUCT)lParam)->lpCreateParams;

		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
	}
	else {
		window = (WindowWin32 *)GetWindowLongPtr(hWnd, GWL_USERDATA);

		if(!window) {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return window->WndProc(hWnd, msg, wParam, lParam);
}

// 윈도우 프로시저
LRESULT WindowWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
	case WM_ACTIVATE:
		if(LOWORD(wParam) == WA_INACTIVE) break;

		Clip(true);
		clipped = true;

		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			clipped = !clipped;

			Clip(clipped);
		}
	case WM_KEYUP:
		AddEventToKeyboardEventQueueForVK((unsigned int)wParam, msg);
		return 0;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		AddEventToMouseEventQueue(msg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		isRunning = false;
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		break;
	case WM_QUIT:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 메시지 루프 시작/종료
void WindowWin32::StartMessageLoop() {
	isRunning = true;
	MSG msg;

	while(isRunning) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		QueryPerformanceCounter(&elapsedTime);
		deltaTime = (double)(elapsedTime.QuadPart-prevElapsedTime.QuadPart)/(double)(queryFrequency.QuadPart);

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

void WindowWin32::AddEventToKeyboardEventQueueForVK(unsigned int vk, UINT msg) {
	Event::Keyboard event;

	static uint32 modifierFlags = 0;

	switch(msg) {
	case WM_KEYDOWN:
		event.type = Event::Keyboard::Down;

		if(vk == VKShift)
			modifierFlags = modifierFlags | ShiftFlags;
		else if(vk == VKControl)
			modifierFlags = modifierFlags | ControlFlags;
		else if(vk == VKOption)
			modifierFlags = modifierFlags | OptionFlags;
		break;
	case WM_KEYUP:
		event.type = Event::Keyboard::Up;
		if(vk == VKShift)
			modifierFlags = modifierFlags ^ ShiftFlags;
		else if(vk == VKControl)
			modifierFlags = modifierFlags ^ ControlFlags;
		else if(vk == VKOption)
			modifierFlags = modifierFlags ^ OptionFlags;
		break;
	}

	event.keyCode = vk;
	event.flags = modifierFlags;

	Event::Emitter<Event::Keyboard>::list.push_back(event);
}


void WindowWin32::AddEventToMouseEventQueue(UINT msg, WPARAM wParam, LPARAM lParam) 
{
	Event::Mouse event;

	// for click count
	static unsigned int clickCount = 0;
	static double prevClickedTime = this->GetElapsedTime();
	static uint32 prevMouseType = Event::Mouse::None;

	double clickedTime = this->GetElapsedTime();

	Point locationInWindow;
	locationInWindow.x = LOWORD(lParam);
	locationInWindow.y = contentSize.height - HIWORD(lParam);

	switch(msg) {
	case WM_LBUTTONDOWN:
		event.type = Event::Mouse::LeftDown;
		if(event.type == prevMouseType && (clickedTime-prevClickedTime) <= 0.250)
			clickCount++;
		else
			clickCount = 1;

		event.clickCount = clickCount;
		prevMouseType = event.type;
		
		break;
	case WM_LBUTTONUP:
		event.type = Event::Mouse::LeftUp;
		event.clickCount = 1;

		break;
	case WM_RBUTTONDOWN:
		event.type = Event::Mouse::RightDown;
		if(event.type == prevMouseType && (clickedTime-prevClickedTime) <= 0.250)
			clickCount++;
		else
			clickCount = 1;

		event.clickCount = clickCount;
		prevMouseType = event.type;

		break;
	case WM_RBUTTONUP:
		event.type = Event::Mouse::RightUp;
		event.clickCount = 1;
		break;
	case WM_MBUTTONDOWN:
		// 추후 지원 예정
		break;
	case WM_MBUTTONUP:
		// 추후 지원 예정
		break;
	case WM_MOUSEMOVE:
		if(wParam & MK_LBUTTON)
			event.type = Event::Mouse::LeftDragged;
		else if(wParam & MK_RBUTTON)
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

void WindowWin32::StopMessageLoop() {
	isRunning = false;
}

Size WindowWin32::GetContentSize() {
	return contentSize;
}

double WindowWin32::GetElapsedTime() {
	double t = (double)elapsedTime.QuadPart / (double)queryFrequency.QuadPart;
	return t;
}

double WindowWin32::FPS() {
	return fps;
}

double WindowWin32::DeltaTime() {
	return deltaTime;
}

// 메모리 해제
WindowWin32::~WindowWin32() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hGLRC);
	ReleaseDC(hWnd, hDC);
}

void WindowWin32::Clip(bool clip)
{
	if(clip)
	{
		RECT clientRect;
		POINT lefttop, rightbottom;

		GetClientRect(hWnd, &clientRect);

		lefttop.x = clientRect.left;
		lefttop.y = clientRect.top;
		rightbottom.x = clientRect.right;
		rightbottom.y = clientRect.bottom;

		ClientToScreen(hWnd, &lefttop);
		ClientToScreen(hWnd, &rightbottom);

		clientRect.left = lefttop.x;
		clientRect.top = lefttop.y;
		clientRect.right = rightbottom.x;
		clientRect.bottom = rightbottom.y;

		ClipCursor(&clientRect);
	}
	else
	{
		ClipCursor(0);
	}
}
}

