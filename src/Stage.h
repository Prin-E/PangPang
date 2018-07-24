#pragma once

#include "Player.h"
#include "Tank.h"

namespace PangPang
{
enum ViewPointMoveDirection
{
	None = 0,
	Left = 1, 
	Right = 1 << 1,
	Top = 1 << 3,
	Bottom = 1 << 4
};

class AIPlayer;

namespace UI
{
class Label;
}

class Stage : public Event::Listener<Event::Mouse>
{
	friend class AIPlayer;
public:
	template <class T>
	Stage(Event::Emitter<Event::Mouse> *emitter, Renderer *renderer, T firstPlayer, T lastPlayer, std::shared_ptr<Map> map, std::shared_ptr<UI::Label> timeCount)
		:	Event::Listener<Event::Mouse>(emitter), currentPlayer(0), elapsedTurnTime(0.0f), delay(0.0f), map(map), playerList(firstPlayer, lastPlayer), alivePlayerList(firstPlayer, lastPlayer)
		, renderer(renderer), md(None), turnIndicator(renderer, Renderer::TankLayer, "dir/turn.json"), timeCount(timeCount), isOver(false)
	{
		for(auto it = playerList.begin(); it != playerList.end(); ++it)
		{
			(*it)->stage = this;
			std::shared_ptr<Tank> tank = (*it)->GetTank();
			tankList.push_back(tank);
			fireEventList.push_back(tank->ConnectFireEvent(boost::bind(&Stage::FireEventHandler, this, _1)));
			destroyEventList.push_back(tank->ConnectDestoryEvent(boost::bind(&Stage::DestoryEventHandler, this, _1)));
		}

		Point view = alivePlayerList[currentPlayer]->GetTank()->GetPosition();
		renderer->SetViewPointWithCenterPoint(view.x, view.y);

		alivePlayerList[currentPlayer]->BeginTurn();

		renderer->SetViewSize(map->GetMapSize());
	}

	bool Update(float dt);

	virtual void Handler(const Event::Mouse::List& list);

private:
	void SetNextPlayer();
	void FireEventHandler(Weapon* weapon);
	void DestoryEventHandler(Tank* weapon);
	bool WeaponEventHandler(Weapon* weapon, float x, float y);
private:
	int currentPlayer;
	float elapsedTurnTime;

	float delay;

	std::shared_ptr<Map> map;
	std::vector<std::shared_ptr<Player> > playerList;
	std::vector<std::shared_ptr<Player> > alivePlayerList;
	std::vector<std::shared_ptr<Tank> > tankList;
	std::vector<boost::signals2::connection> fireEventList;
	std::vector<boost::signals2::connection> destroyEventList;
	boost::signals2::connection weaponEvent;

	static const float timeLimit;

	Renderer *renderer;
	unsigned int md;

	Sprite turnIndicator;

	std::shared_ptr<UI::Label> timeCount;

	bool isOver;
};
}

