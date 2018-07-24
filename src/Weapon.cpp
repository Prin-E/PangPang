#include "Common.h"
#include "Weapon.h"
#include "Map.h"
#include "Resource.h"

namespace PangPang
{
Weapon::Weapon(Map* map, Renderer* renderer)
	:	damage(0), numProjectiles(1), isActivated(false), power(0.0f), elapsedTime(0.0f), origin_x(0.0f), origin_y(0.0f), prev_x(origin_x), prev_y(origin_y), map(map), renderer(renderer)
{
}

Weapon::Weapon(Map* map, Renderer* renderer, const char* path)
	:	isActivated(false), power(0.0f), elapsedTime(0.0f), origin_x(0.0f), origin_y(0.0f), map(map), renderer(renderer)
{
	Load(path);
}

void Weapon::Load(const char* path)
{
	Json::Value root;
	Json::Reader reader;

	ResourcePtr rsrc = Resource::Open(path);

	char* begin = (char*)rsrc->ptr;

	reader.parse(begin, begin + rsrc->size, root);

	damage = root.get("damage", 0).asInt();

	numProjectiles = root.get("numProjectiles", 1).asInt();
	
	sprites.reserve(numProjectiles);

	std::string spritePath = root.get("sprite", "").asString();

	for(int i = 0; i < numProjectiles; ++i)
	{
		std::shared_ptr<Sprite> temp(new Sprite(renderer, Renderer::WeaponLayer, spritePath.c_str()));
		temp->SetVisible(false);
		sprites.push_back(temp);
	}
}

void Weapon::Fire(Point pos, float angle_, float power_)
{
	angle = angle_;
	power = power_;

	prev_x = origin_x = pos.x;
	prev_y = origin_y = pos.y;

	sprites[0]->SetPosition(pos);
	sprites[0]->SetVisible(true);
	elapsedTime = 0.0f;

	isActivated = true;
}

bool Weapon::Update(float dt)
{
	if(isActivated)
	{
		elapsedTime += dt;

		const float g = 9.8f * 100;
		const float v0 = 500.0f;

		current_x = origin_x + power * cos(angle) * elapsedTime * v0;
		current_y = origin_y + power * sin(angle) * elapsedTime * v0 - 0.5f * g * elapsedTime * elapsedTime;

		Terrain terrain = map->GetTerrain();
		Size size = map->GetMapSize();

		if(current_x < -100 || size.width + 100 < current_x ||
			current_y < 0 || size.height + 500 < current_y)
		{
			isActivated = false;
			sprites[0]->SetVisible(false);
			std::cout << "Disabled" << std::endl;
		}

		sprites[0]->SetPosition(Point(current_x, current_y));

		bool boom = /*terrain(current_x, current_y) || */*UpdateEvent(this, current_x, current_y);
		
		if(boom)
		{
			isActivated = false;
			sprites[0]->SetVisible(false);
			UpdateEvent.disconnect_all_slots();

			map->DigHole(Circle(Point(current_x, current_y), 100));

			std::cout << "Boom" << std::endl;
		}
		else
		{
			sprites[0]->Update(dt);
		}

		return true;
	}
	return false;
}

void Weapon::DrawForCollisionTest()
{
	((Drawable*)(sprites[0].get()))->Draw();
}
Rect Weapon::GetBoundingRect()
{
	Rect rect1;
	rect1.origin = sprites[0]->GetPosition();
	rect1.size = sprites[0]->GetSize();
	return rect1;
}

boost::signals2::connection Weapon::ConnectUpdateEvent(boost::function<bool(Weapon* weapon, float x, float y)> fn)
{
	return UpdateEvent.connect(fn);
}
}

