#pragma once

#include <boost/thread/once.hpp>

#include "State.h"

namespace PangPang
{
class WaitingRoom : public State
{
public:
	WaitingRoom(StateManager* ptr) : State(ptr), totalTime(0.0f) {}
public:
	virtual void OnEnter();
	virtual void OnExit();
	virtual void Update(float deltaTime);
private:
	float totalTime;
};
}

