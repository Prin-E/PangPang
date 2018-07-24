#pragma once

#include "Player.h"
#include "Event.h"

namespace PangPang
{
namespace UI
{
class Bar;
class AngleView;
}
class LocalPlayer : public Player, private Event::Listener<Event::Keyboard>
{
public:
	LocalPlayer(std::shared_ptr<Tank> tank, Event::Emitter<Event::Keyboard>* controller, std::shared_ptr<UI::Bar> lifeGuage, std::shared_ptr<UI::Bar> powerGuage, std::shared_ptr<UI::Bar> moveGuage, std::shared_ptr<UI::AngleView> angleGuage);
private:
	virtual void OnBegin(Tank& tank);
	virtual void OnEnd(Tank& tank);
	virtual bool OnUpdate(Tank& tank, float dt);
	virtual void Handler(const Event::Keyboard::List& list);
private:
	bool leftPressed;
	bool rightPressed;
	bool upPressed;
	bool downPressed;

	bool spacePressed;
	float spacePeriod;

	float angle;

	bool fire;

	int direction;

	std::shared_ptr<UI::Bar> lifeGuage;
	std::shared_ptr<UI::Bar> powerGuage;
	std::shared_ptr<UI::Bar> moveGuage;
	std::shared_ptr<UI::AngleView> angleGuage;
};
}

