#include "Common.h"
#include "LocalPlayer.h"
#include "Bar.h"
#include "AngleView.h"

namespace PangPang
{
LocalPlayer::LocalPlayer(std::shared_ptr<Tank> tank, Event::Emitter<Event::Keyboard>* controller, std::shared_ptr<UI::Bar> lifeGuage, std::shared_ptr<UI::Bar> powerGuage, std::shared_ptr<UI::Bar> moveGuage, std::shared_ptr<UI::AngleView> angleGuage)
	:	Player(tank), Event::Listener<Event::Keyboard>(controller), leftPressed(false), rightPressed(false), upPressed(false), downPressed(false), spacePressed(false), spacePeriod(0.0f), angle(0.0f), fire(false), direction(Tank::Stopped),
	lifeGuage(lifeGuage), powerGuage(powerGuage), moveGuage(moveGuage), angleGuage(angleGuage)
{

}

void LocalPlayer::OnBegin(Tank& tank)
{
	lifeGuage->SetValue((float)tank.GetLife() / 1000 * 100);
	powerGuage->SetValue(0.0f);
	moveGuage->SetValue(tank.GetRemainMovingTime() / Tank::maxMovingTime * 100.0f);

	float maxAngle = tank.GetMaxAngle();
	float minAngle = tank.GetMinAngle();

	angle = (angle > maxAngle)? maxAngle - 0.1f : angle;
	angle = (angle < minAngle)? minAngle + 0.1f : angle;

	angleGuage->SetAngle(angle * 180.0f / 3.141592f);

	angleGuage->SetMaxAngle(maxAngle * 180.0f / 3.141592f);
	angleGuage->SetMinAngle(minAngle * 180.0f / 3.141592f);
}

void LocalPlayer::OnEnd(Tank& tank)
{
	leftPressed = false;
	rightPressed = false;
	spacePressed = false;
	fire = false;
	spacePeriod = 0.0f;
	direction = Tank::Stopped;
	tank.Move(Tank::Stopped);
}

bool LocalPlayer::OnUpdate(Tank& tank, float dt)
{
	bool busy = false;
	if(leftPressed || rightPressed && !spacePressed)
	{
		tank.Move((Tank::Direction)direction);
		moveGuage->SetValue(tank.GetRemainMovingTime() / Tank::maxMovingTime * 100.0f);

		float maxAngle = tank.GetMaxAngle();
		float minAngle = tank.GetMinAngle();

		angle = (angle > maxAngle)? maxAngle - 0.1f : angle;
		angle = (angle < minAngle)? minAngle + 0.1f : angle;

		angleGuage->SetAngle(angle * 180.0f / 3.141592f);

		angleGuage->SetMaxAngle(tank.GetMaxAngle() * 180.0f / 3.141592f);
		angleGuage->SetMinAngle(tank.GetMinAngle() * 180.0f / 3.141592f);
	}
	else
	{
		tank.Move(Tank::Stopped);
	}

	if(upPressed)
	{
		if(tank.GetDirection() == Tank::Left)
			angle -= dt * 0.5f;
		else
			angle += dt * 0.5f;

		float maxAngle = tank.GetMaxAngle();
		float minAngle = tank.GetMinAngle();

		angle = (angle > maxAngle)? maxAngle : angle;
		angle = (angle < minAngle)? minAngle : angle;

		angleGuage->SetAngle(angle * 180.0f / 3.141592f);
	}
	else if(downPressed)
	{
		if(tank.GetDirection() == Tank::Left)
			angle += dt * 0.5f;
		else
			angle -= dt * 0.5f;

		float maxAngle = tank.GetMaxAngle();
		float minAngle = tank.GetMinAngle();

		angle = (angle > maxAngle)? maxAngle : angle;
		angle = (angle < minAngle)? minAngle : angle;

		angleGuage->SetAngle(angle * 180.0f / 3.141592f);
	}

	if(spacePressed && !fire)
	{
		spacePeriod += dt;
		spacePeriod = (spacePeriod > 3.0f)? 3.0f : spacePeriod;

		powerGuage->SetValue(spacePeriod / 3.0f * 100.0f);

		busy |= true;
	}

	if(fire)
	{
		tank.Fire(angle, spacePeriod);
		fire = false;
		EndMyTurn();
	}

	return busy;
}

void LocalPlayer::Handler(const Event::Keyboard::List& list)
{
	if(!IsMyTurn()) return;
	for(auto it = list.begin(); it != list.end(); ++it)
	{
		const Event::Keyboard& event = (*it);

		switch(event.keyCode)
		{
		case VKLeft:
			if(event.type == Event::Keyboard::Down && !leftPressed)
			{
				direction += Tank::Left;
				leftPressed = true;
			}
			else if(event.type == Event::Keyboard::Up && leftPressed)
			{
				direction -= Tank::Left;
				leftPressed = false;
			}
			break;
		case VKRight:
			if(event.type == Event::Keyboard::Down && !rightPressed)
			{
				direction += Tank::Right;
				rightPressed = true;
			}
			else if(event.type == Event::Keyboard::Up && rightPressed)
			{
				direction -= Tank::Right;
				rightPressed = false;
			}
			break;
		case VKUp:
			if(event.type == Event::Keyboard::Down && !upPressed)
			{
				upPressed = true;
			}
			else if(event.type == Event::Keyboard::Up && upPressed)
			{
				upPressed = false;
			}
			break;
		case VKDown:
			if(event.type == Event::Keyboard::Down && !downPressed)
			{
				downPressed = true;
			}
			else if(event.type == Event::Keyboard::Up && downPressed)
			{
				downPressed = false;
			}
			break;
		case VKSpace:
			if(event.type == Event::Keyboard::Down && !spacePressed)
			{
				spacePeriod = 0.0f;
				spacePressed = true;
				std::cout << "space pressed" << std::endl;
			}
			else if(event.type == Event::Keyboard::Up && spacePressed)
			{
				spacePressed = false;
				std::cout << "Space bar was pressed for " << spacePeriod << " seconds." << std::endl;

				fire = true;
			}
			break;
		}
	}
}
}

