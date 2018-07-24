//
//  WindowCocoa.mm
//  PangPang
//
//  Created by 이현우 on 10. 12. 17..
//  Copyright 2010 서울시립대학교. All rights reserved.
//

#import "WindowCocoa.h"
#import <Cocoa/Cocoa.h>

namespace PangPang {
// 초기화 메서드
WindowCocoa::WindowCocoa(const wchar_t* title, Size size)
:	windowTitle(title)
{
	NSRect contentRect = NSMakeRect(0, 0, size.width, size.height);
	
	NSWindow *window = [[NSWindow alloc] initWithContentRect: contentRect 
												   styleMask: NSTitledWindowMask
													 backing: NSBackingStoreBuffered
													   defer: NO];
	
	// wchar_t to unichar
	wchar_t *wcs = (wchar_t *)windowTitle.c_str();
	unichar *str = new unichar[wcslen(wcs)+1];
	for(int i = 0; i < wcslen(wcs)+1; i++)
		str[i] = (unichar)wcs[i];
	
	// 윈도우 타이틀 지정
	[window setTitle: [NSString stringWithCharacters: str length: wcslen(wcs)+1]];
	delete str;
	
	// 타이머 초기화
	timerDate = (NSDate *)[[NSDate alloc] initWithTimeIntervalSinceNow: 0.0];
	
	// 컨텍스트 초기화
	InitContext();
	
	prevElapsedTime = (double)[(NSDate *)timerDate timeIntervalSinceNow];
}

// 컨텍스트 초기화
void WindowCocoa::InitContext() {
	NSOpenGLPixelFormatAttribute attributes[32];
	NSOpenGLPixelFormat *pixelFormat;
	
	// 창 모드용
	if(1) {
		int i = 0;
		attributes[i++] = NSOpenGLPFANoRecovery;
		attributes[i++] = NSOpenGLPFADoubleBuffer;
		attributes[i++] = NSOpenGLPFAAccelerated;
		attributes[i++] = NSOpenGLPFADepthSize;
		attributes[i++] = 16;
		attributes[i++] = NSOpenGLPFAColorSize;
		attributes[i++] = 32;
		attributes[i++] = NSOpenGLPFAAlphaSize;
		attributes[i++] = 16;
		attributes[i++] = 0;
		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
		if(pixelFormat) {
			openGLContext = [[NSOpenGLContext alloc] initWithFormat: pixelFormat shareContext: nil];
			cglContext = [(NSOpenGLContext *)openGLContext CGLContextObj];
			[pixelFormat release];
		}
		else {
			fprintf(stderr, "OpenGL Pixel Format 초기화 실패!\n");
		}
		if(!openGLContext)
			fprintf(stderr, "OpenGL Context 초기화 실패!\n");
	}
	
	openGLView = (NSOpenGLView *)[[NSOpenGLView alloc] init];
	[(NSOpenGLView *)openGLView setOpenGLContext: (NSOpenGLContext *)openGLContext];
	CGLSetCurrentContext((CGLContextObj)cglContext);
	
	InitOpenGLView(); // OpenGL 뷰 초기화
}

void WindowCocoa::InitOpenGLView() {
	glClearColor(0, 0, 0, 0);
	
	glViewport(0, 0, windowRect.size.width, windowRect.size.height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (double)windowRect.size.width, 0, (double)windowRect.size.height, 100.0, -100.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WindowCocoa::SwapBuffers()
{
	CGLFlushDrawable((CGLContextObj)cglContext);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// 메시지 루프 시작/종료
void WindowCocoa::StartMessageLoop() {
	[(NSWindow *)window setIsVisible: YES];
	[(NSWindow *)window setAcceptsMouseMovedEvents: YES];
	[(NSWindow *)window makeKeyAndOrderFront: (NSWindow *)window];
	
	[(NSWindow *)window setContentView: (NSOpenGLView *)openGLView];
	[(NSOpenGLView *)openGLView release];
	
	isRunning = true;
	
	NSEvent *event;
	while(isRunning) {
		event = [NSApp nextEventMatchingMask: NSAnyEventMask untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES];
		if(event) {
			switch([event type]) {
				case NSKeyDown:
				case NSKeyUp:
					this->AddEventToKeyboardEventQueueForVK((void *)event);
					break;
			}
		}
		else {
			// 이벤트 분배
			Event::Emitter<Event::Keyboard>::Emit();
			Event::Emitter<Event::Mouse>::Emit();
		
			// 게임 업데이트
			[(NSOpenGLContext *)openGLContext update];
			UpdateGame(deltaTime);
		
			// FPS 계산
			elapsedTime = (double)[(NSDate *)timerDate timeIntervalSinceNow];
			deltaTime = elapsedTime = prevElapsedTime;
			
			prevElapsedTime = elapsedTime;
			fps = 1.0 / deltaTime;
		}
	}
}

void WindowCocoa::AddEventToKeyboardEventQueueForVK(void *event) {
	Event::Keyboard theEvent;
	NSEvent *nsEvent = (NSEvent *)event;
	
	uint32 flags = nsEvent.modifierFlags;
	static uint32 modifierFlags = 0;
	
	switch(nsEvent.type) {
		case NSKeyDown:
			theEvent.type = Event::Keyboard::Down;
			if(flags == NSShiftKeyMask)
				modifierFlags = modifierFlags | ShiftFlags;
			else if(flags == NSControlKeyMask)
				modifierFlags = modifierFlags | ControlFlags;
			else if(flags == NSAlternateKeyMask)
				modifierFlags = modifierFlags | OptionFlags;
			break;
		case NSKeyUp:
			theEvent.type = Event::Keyboard::Up;
			if(flags == NSShiftKeyMask)
				modifierFlags = modifierFlags ^ ShiftFlags;
			else if(flags == NSControlKeyMask)
				modifierFlags = modifierFlags ^ ControlFlags;
			else if(flags == NSAlternateKeyMask)
				modifierFlags = modifierFlags ^ OptionFlags;
			break;
	}
	
	NSString *str = [nsEvent characters];
	::unichar character = [str characterAtIndex: 0];
	
	if(character >= 'a' && character <= 'z' && character >= 'A' && character <= 'Z')
		theEvent.keyCode = (unsigned int)character;
	else
		theEvent.keyCode = nsEvent.keyCode;
	
	theEvent.flags = modifierFlags;
	
	Event::Emitter<Event::Keyboard>::list.push_back(theEvent);
}

void WindowCocoa::StopMessageLoop() {
	isRunning = false;
}


double WindowCocoa::GetElapsedTime() {
	return elapsedTime;
}

double WindowCocoa::FPS() {
	return fps;
}

double WindowCocoa::DeltaTime() {
	return deltaTime;
}

// 메모리 해제
WindowCocoa::~WindowCocoa() {
	[(NSWindow *)window release];
	window = nil;
	openGLContext = nil;
	cglContext = nil;
	[(NSDate *)timerDate release];
	timerDate = nil;
}
}
