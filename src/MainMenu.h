#pragma once

#include "GameEngine.h"
#include <boost/thread/once.hpp>
#include "State.h"

namespace PangPang
{
class MainMenu : public State
{
	friend class StateManager;
public:
	MainMenu(StateManager* ptr);
public:
	virtual void OnEnter();
	virtual void OnExit();
	virtual void Update(float deltaTime);
private:
	void StartGame(uint32 tag);
private:
	Renderer *renderer;

	UI::UIManager uiManager;
	std::shared_ptr<UI::ImageView> backgroundImage;
	std::shared_ptr<UI::Button> pvpButton;
	std::shared_ptr<UI::Button> pvaiButton;

	std::shared_ptr<UI::Label> versionLabel;
	std::shared_ptr<UI::Label> copyrightLabel;
};
}

