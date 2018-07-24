#pragma once

#include <boost/signals2.hpp>
#include <boost/function.hpp>

#include "Sprite.h"
#include "Map.h"
#include "Weapon.h"
#include "Collidable.h"
#include "Types.h"

namespace PangPang
{
class Tank
{
public:
	enum Direction {Left = -1, Stopped = 0, Right = 1};
	enum WeaponType {Primary = 0, Secondary};
	static const float maxMovingTime;
public:
	Tank(Renderer* renderer, const char* path, std::shared_ptr<Map> map, Point initialPos);
public:
	void Move(Direction dir);
	void Fire(float angle, float power);
	void SetWeapon(WeaponType type);
	int GetLife() const
	{
		return life;
	}
	Point GetPosition() const
	{
		return Point(x, y);
	}
	float GetRemainMovingTime() const
	{
		return remainMovingTime;
	}
	float GetTankAngle() const
	{
		return angle;
	}
	float GetMinAngle() const;
	float GetMaxAngle() const;
	Direction GetDirection() const
	{
		return sprite.GetHorizontalFlipped()? Left : Right;
	}
	float GetTankAngleOnPoint(float p_x, float p_y = 0) const;
	Point GetWeaponPosition() const;
	bool Update(float dt);

	void Damage(int damage)
	{
		life -= damage;
	}

	boost::signals2::connection ConnectFireEvent(boost::function<void(Weapon* weapon)> fn);
	boost::signals2::connection ConnectDestoryEvent(boost::function<void(Tank* tank)> fn);
private:
	Sprite sprite;
	std::shared_ptr<Weapon> weapon[2];

	std::shared_ptr<Map> map;

	float x, y;
	Direction direction;
	float angle;

	float minAngle;
	float maxAngle;

	float verticalSpeed;

	bool isDead;
	int life;

	float remainMovingTime;

	WeaponType selectedWeapon;

	boost::signals2::signal<void(Weapon* weapon)> onFireSig;
	boost::signals2::signal<void(Tank* tank)> onDestorySig;

	static std::vector<Tank*> tankList;
};
}

