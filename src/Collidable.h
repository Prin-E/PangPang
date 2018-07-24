#pragma once

#include "Types.h"

namespace PangPang
{
class Drawable;
class Collidable;

bool TestCollision(Collidable* lhs, Collidable* rhs);

class Collidable
{
	friend bool TestCollision(Collidable* lhs, Collidable* rhs);
protected:
	Collidable() {};
private:
	virtual void DrawForCollisionTest() = 0;
	virtual Rect GetBoundingRect() = 0;
};
}

