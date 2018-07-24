#include "Common.h"
#include "Player.h"

namespace PangPang
{
Player::Player(std::shared_ptr<Tank> tank)
	:	tank(tank), myTurn(false)
{

}
}

