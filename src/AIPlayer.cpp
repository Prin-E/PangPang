#include "Common.h"

#include "Stage.h"
#include "AIPlayer.h"

namespace PangPang
{
AIPlayer::AIPlayer(std::shared_ptr<Tank> tank)
	:	Player(tank)
{

}

void AIPlayer::OnBegin(Tank& tank)
{
	Stage* stage = GetStage();

	auto it = std::find_if(stage->alivePlayerList.begin(), stage->alivePlayerList.end(),
		[this](std::shared_ptr<Player> p)
	{
		return p.get() != this;
	});

	opponent = (*it).get();

	delay = 0.0f;
	found = false;

	std::cout << "AIPlayer: " << opponent << " is opponent of this player." << std::endl;
}
void AIPlayer::OnEnd(Tank& tank)
{

}
bool AIPlayer::OnUpdate(Tank& tank, float dt)
{
	delay += dt;

	if(found)
	{
		if(delay > 2.0f)
		{
			tank.Fire(angle, power);
			EndMyTurn();
		}
		return false;
	}

	Point opponent_pos = opponent->GetTank()->GetPosition();
	Point pos = tank.GetPosition();

	if(opponent_pos - pos < 100)
	{
		if(opponent_pos.x < pos.x)
			tank.Move(Tank::Right);
		else
			tank.Move(Tank::Left);
		return false;
	}

	if(opponent_pos.x < pos.x && tank.GetDirection() != Tank::Left)
	{
		tank.Move(Tank::Left);
		return false;
	}
	else if(opponent_pos.x > pos.x && tank.GetDirection() != Tank::Right)
	{
		tank.Move(Tank::Right);
		return false;
	}

	tank.Move(Tank::Stopped);


	float maxAngle = tank.GetMaxAngle();
	float minAngle = tank.GetMinAngle();

	float offsetAngle = tank.GetTankAngle();

	if(tank.GetDirection() == Left)
	{
		offsetAngle += 3.141592f - 0.9f;
	}
	else
	{
		offsetAngle += 0.9f;
	}

	const float offset = 64.0f;

	float offset_x = cos(offsetAngle) * offset;
	float offset_y = sin(offsetAngle) * offset;

 	pos.x += offset_x;
 	pos.y += offset_y;

	float dist_x = opponent_pos.x - pos.x;
	float dist_y = opponent_pos.y - pos.y;

	const float g = 9.8f * 100;

	for(angle = maxAngle; angle >= minAngle; angle -= 0.01)
	{
		power = sqrt(0.5f * g * dist_x * dist_x / (cos(angle) * cos(angle) * (tan(angle) * dist_x - dist_y)));
		std::cout << angle << " " << power << std::endl;
		power = power / 500.0f - 0.7f;
		if(0.0f < power && power < 3.0f)
		{
			std::cout << "found" << std::endl;
			found = true;
			break;
		}
	}

	return false;
}
}

