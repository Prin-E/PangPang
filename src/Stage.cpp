#include "Common.h"
#include "Stage.h"
#include "Label.h"

#include <cmath>

#include <boost/lexical_cast.hpp>

namespace PangPang
{
const float Stage::timeLimit = 10.0f;

bool Stage::Update(float dt)
{
	// 시점 이동
	Size mapSize = map->GetMapSize();
	float vpMoveDistance = 300.0f * dt;
	float vx = renderer->GetViewPointX(), vy = renderer->GetViewPointY();

	if(md & Left)
		vx -= vpMoveDistance;
	if(md & Right)
		vx += vpMoveDistance;
	if(md & Bottom)
		vy -= vpMoveDistance;
	if(md & Top)
		vy += vpMoveDistance;
	renderer->SetViewPoint(vx, vy);

	// 탱크 업데이트
	bool busy_tank = false;
	for(auto it = tankList.begin(); it != tankList.end(); ++it)
	{
		bool busy = (*it)->Update(dt);
		busy_tank |= busy;

		if(busy)
		{
			Point wp = (*it)->GetWeaponPosition();
			if(wp.x != 0 && wp.y != 0)
			{
				std::cout << wp.x << " " << wp.y << std::endl;
				renderer->SetViewPointWithCenterPoint(wp.x, wp.y);
			}
		}
	}

	if(busy_tank) return true;

	if(isOver)
	{
		delay += dt;
		if(delay > 1.0f)
		{
			return false;
		}
		return true;
	}

	if(alivePlayerList.size() <= 1)
	{
		if(!alivePlayerList.empty())
		{
			std::cout << "winner: " << alivePlayerList[0].get() << std::endl;
		}
		isOver = true;
		delay = 0;
		return true;
	}

	Point tank = alivePlayerList[currentPlayer]->GetTank()->GetPosition();
	tank.y += 64;
	turnIndicator.SetPosition(tank);
	turnIndicator.Update(dt);

	std::wstring remain = boost::lexical_cast<std::wstring>((int)ceil(timeLimit - elapsedTurnTime));
	timeCount->SetText(remain);

	if(elapsedTurnTime > timeLimit || !alivePlayerList[currentPlayer]->IsMyTurn())
	{
		delay += dt;
		if(delay > 1.0f)
		{
			SetNextPlayer();
			delay = 0.0;
		}
		else
			return true;
	}

	bool busy = alivePlayerList[currentPlayer]->Update(dt);

	if(busy) return true;

	elapsedTurnTime += dt;

	return true;
}

void Stage::SetNextPlayer()
{
	if(alivePlayerList[currentPlayer]->IsMyTurn())
		alivePlayerList[currentPlayer]->EndTurn();

	elapsedTurnTime = 0.0f;
	++currentPlayer %= alivePlayerList.size();

	std::cout << "Next player: " << currentPlayer << std::endl;

	Point view = alivePlayerList[currentPlayer]->GetTank()->GetPosition();
	renderer->SetViewPointWithCenterPoint(view.x, view.y, 0.5f);
	alivePlayerList[currentPlayer]->BeginTurn();
}

void Stage::FireEventHandler(Weapon* weapon)
{
// 	for(auto it = tankList.begin(); it != tankList.end(); ++it)
// 	{
// 		if(TestCollision((*it).get(), weapon))
// 			std::cout << "hit" << std::endl;
// 	}

	weaponEvent = weapon->ConnectUpdateEvent(boost::bind(&Stage::WeaponEventHandler, this, _1, _2, _3));
}

void Stage::DestoryEventHandler(Tank* tank)
{
// 	auto new_end = std::remove_if(alivePlayerList.begin(), alivePlayerList.end(),
// 	[=](std::shared_ptr<Player> player)->bool
// 	{
// 		Tank* dst = player->GetTank().get();
// 		return dst == tank;
// 	});

	for(auto it = alivePlayerList.begin(); it != alivePlayerList.end(); )
	{
		if((*it)->GetTank().get() == tank)
			it = alivePlayerList.erase(it);
		else
			++it;
	}

	//alivePlayerList.erase(new_end, alivePlayerList.end());
}

bool Stage::WeaponEventHandler(Weapon* weapon, float x, float y)
{
	if(TestCollision(map.get(), weapon))
	{
		std::cout << "hit" << std::endl;
		std::cout << x << " " <<y << std::endl;
		weaponEvent.disconnect();

		for(auto it = tankList.begin(); it != tankList.end(); ++it)
		{
			float length = (*it)->GetPosition() - Point(x,y);
			if(length < 100.0f)
			{
				float damage = (float)weapon->GetDamage() * (1 - length / 100);
				(*it)->Damage(damage);
				std::cout << "damage: " << damage << std::endl;
			}
		}
		return true;
	}
	return false;
}

void Stage::Handler(const Event::Mouse::List& list)
{
	for(auto it = list.begin(); it != list.end(); it++) {
		const Event::Mouse &mouseEvent = (*it);
		
		md = None;

		if(mouseEvent.locationInWindow.x < 10)
			md |= Left;
		else if(mouseEvent.locationInWindow.x > 790)
			md |= Right;
		if(mouseEvent.locationInWindow.y < 10)
			md |= Bottom;
		else if(mouseEvent.locationInWindow.y > 590)
			md |= Top;
	}
}
}