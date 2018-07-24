#pragma once

#include "Player.h"

namespace PangPang
{
class AIPlayer : public Player
{
public:
	AIPlayer(std::shared_ptr<Tank> tank);
private:
	virtual void OnBegin(Tank& tank);
	virtual void OnEnd(Tank& tank);
	virtual bool OnUpdate(Tank& tank, float dt);
private:
	Player* opponent;
	
	float delay;
	
	bool found;
	float angle;
	float power;
};
}

