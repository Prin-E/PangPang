#include "Common.h"
#include "WaitingRoom.h"
#include "MainMenu.h"

namespace PangPang
{
void WaitingRoom::OnEnter()
{
	std::cerr << __FUNCTION__ << std::endl;
}

void WaitingRoom::OnExit()
{
	std::cerr << __FUNCTION__ << std::endl;

	totalTime = 0.0f;
}

void WaitingRoom::Update(float deltaTime)
{
	static const char* funcName = __FUNCTION__;

	totalTime += deltaTime;

	if(totalTime > 5.0f)
		SetState<MainMenu>();
}
}

