#pragma once

#include "Tank.h"

namespace PangPang
{
class Stage;
class AIPlayer;

class Player
{
	friend class Stage;
	friend class AIPlayer;
public:
	Player(std::shared_ptr<Tank> tank);
	virtual ~Player() {};
private:
	std::shared_ptr<Tank> GetTank() const
	{
		return tank;
	}
	void BeginTurn()
	{
		myTurn = true;
		OnBegin(*tank.get());
	}
	void EndTurn()
	{
		OnEnd(*tank.get());
		myTurn = false;
	}
	bool Update(float dt)
	{
		return OnUpdate(*tank.get(), dt);
	}
protected:
	bool IsMyTurn() const
	{
		return myTurn;
	}
	void EndMyTurn()
	{
		EndTurn();
	}
	Stage* GetStage() const
	{
		return stage;
	}
private:
	virtual void OnBegin(Tank& tank) = 0;
	virtual void OnEnd(Tank& tank) = 0;
	virtual bool OnUpdate(Tank& tank, float dt) = 0;
private:
	std::shared_ptr<Tank> tank;
	bool myTurn;
	Stage* stage;
};
}

