#pragma once

#include "GameEngine.h"
#include <boost/thread/once.hpp>
#include "State.h"

namespace PangPang
{
class Stage;
class InGameState : public State
{
	friend class StateManager;
public:
	enum GameType
	{
		PvAI,
		PvP
	};
public:
	InGameState(StateManager* ptr);
private:
	void OnEnter(GameType type);
	virtual void OnExit();
	virtual void Update(float deltaTime);
private:
	float totalTime;
	Renderer* renderer;
	std::shared_ptr<Stage> stage;

	UI::UIManager uiManager;
	std::shared_ptr<UI::Bar> bar1, bar2, bar3;
	std::shared_ptr<UI::AngleView> angleView;
	std::shared_ptr<UI::ImageView> imageView;
	std::shared_ptr<UI::Label> timerLabel;
};
}

