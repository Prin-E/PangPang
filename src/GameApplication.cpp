#include "Common.h"
#include "GameApplication.h"

namespace PangPang
{
GameApplication::GameApplication(int argc, char *argv[], const wchar_t* title, Size size)
	:	Window(title, size)
{

}

GameApplication::~GameApplication()
{

}

void GameApplication::Run() {
	StartMessageLoop();
}

void GameApplication::Stop() {
	StopMessageLoop();
}
}

