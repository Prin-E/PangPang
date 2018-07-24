#pragma once

#include "Sprite.h"
#include "Collidable.h"

namespace PangPang
{
class Map;

class Weapon : public Collidable
{
public:
	Weapon(Map* map, Renderer* renderer);
	Weapon(Map* map, Renderer* renderer, const char* path);
	virtual ~Weapon() {};
public:
	void Load(const char* path);
	void Fire(Point pos, float angle, float power);
	int GetDamage() const
	{
		return damage;
	}
	Point GetPosition() const
	{
		if(isActivated)
			return Point(current_x, current_y);
		else
			return Point();
	}
	bool Update(float dt);

	boost::signals2::connection ConnectUpdateEvent(boost::function<bool(Weapon* weapon, float x, float y)> fn);
private:
	void DrawForCollisionTest();
	Rect GetBoundingRect();
private:
	std::vector<std::shared_ptr<Sprite> > sprites;
	int damage;
	int numProjectiles;

	bool isActivated;
	float angle;
	float power;
	float elapsedTime;
	float origin_x, origin_y;
	float current_x, current_y;
	float prev_x, prev_y;

	Map* map;
	Renderer* renderer;

	boost::signals2::signal<bool(Weapon* weapon, float x, float y)> UpdateEvent;
};
}

