#include "Common.h"
#include "Tank.h"
#include "Resource.h"

namespace PangPang
{
const float Tank::maxMovingTime = 3.0f;

Tank::Tank(Renderer* renderer, const char* path, std::shared_ptr<Map> map, Point initialPos)
	:	sprite(renderer, Renderer::TankLayer), map(map), x(initialPos.x), y(initialPos.y), direction(Stopped), angle(0.0f), minAngle(0.0f), maxAngle(0.0f), verticalSpeed(0.0f), isDead(false), life(0), remainMovingTime(maxMovingTime), selectedWeapon(Primary)
{
	Json::Value root;
	Json::Reader reader;

	ResourcePtr rsrc = Resource::Open(path);

	char* begin = (char*)rsrc->ptr;

	if(!reader.parse(begin, begin + rsrc->size, root))
	{
		std::cerr << "Tank::Tank - Json 파일 파싱 실패" << std::endl;
	}

	life = root.get("life", -1).asInt();

	std::string sprite_path = root.get("sprite", "").asString();

	sprite.Load(sprite_path.c_str());

	// both of below should not be null string.
	std::string weapon_path_1 = root.get("first_weapon", "").asString();
	std::string weapon_path_2 = root.get("second_weapon", "").asString();

	weapon[0].reset(new Weapon(map.get(), renderer, weapon_path_1.c_str()));
	weapon[1].reset(new Weapon(map.get(), renderer, weapon_path_2.c_str()));

	sprite.SetAnimation("Default");
	sprite.SetPosition(initialPos);
}

void Tank::Move(Direction dir)
{
	if(direction != dir)
	{
		if(dir == Stopped)
			sprite.SetAnimation("Default");
		else
			sprite.SetAnimation("Move");
	}

	direction = dir;

	if(direction == Left)
		sprite.SetHorizontalFlipped(true);
	else if(direction == Right)
		sprite.SetHorizontalFlipped(false);
}

static int GetTop(Terrain& terrain, Size mapSize, int x, int init_y)
{
	if(terrain(x, init_y))
		for(; init_y < mapSize.height - 1 && terrain(x, init_y + 1); ++init_y);	// 위로 탐색
	else
		for(; init_y > 0 && !terrain(x, init_y); --init_y);						// 아래로 탐색

	return init_y;
}

void Tank::Fire(float angle, float power)
{
	float maxAngle = GetMaxAngle();
	float minAngle = GetMinAngle();

	angle = (angle > maxAngle)? maxAngle : angle;
	angle = (angle < minAngle)? minAngle : angle;

	float weaponAngle = this->angle;
	float offsetAngle = this->angle;

	if(GetDirection() == Left)
	{
		weaponAngle += 3.141592f - angle;
		offsetAngle += 3.141592f - 0.9f;
	}
	else
	{
		weaponAngle += angle;
		offsetAngle += 0.9f;
	}
	onFireSig(weapon[selectedWeapon].get());

	const float offset = 64.0f;

	float offset_x = cos(offsetAngle) * offset;
	float offset_y = sin(offsetAngle) * offset;

	weapon[selectedWeapon]->Fire(Point(x + offset_x, y + offset_y), angle, power + 0.3f);

	std::cout << "Fired: " << angle * 180 / 3.141592 << std::endl;

	remainMovingTime = maxMovingTime;
}


float Tank::GetMinAngle() const
{
	float weaponAngle = this->angle;

	if(GetDirection() == Left)
	{
		weaponAngle += 3.141592f - 3.141592f / 6;
	}
	else
	{
		weaponAngle += 3.141592f / 6;
	}

	return weaponAngle - 3.141592f / 10;
}
float Tank::GetMaxAngle() const
{
	float weaponAngle = this->angle;

	if(GetDirection() == Left)
	{
		weaponAngle += 3.141592f - 3.141592f / 6;
	}
	else
	{
		weaponAngle += 3.141592f / 4;
	}

	return weaponAngle + 3.141592f / 8;
}

float Tank::GetTankAngleOnPoint(float p_x, float p_y) const
{
	Terrain terrain = map->GetTerrain();
	Size mapSize = map->GetMapSize();

	p_y = GetTop(terrain, mapSize, p_x, p_y);

	// 아래는 각도 조절
	int center_y = p_y;

	int left_y1 = p_y, left_y2 = p_y;
	int right_y1 = p_y, right_y2 = p_y;

	// 기울기 구할 때 필요한 샘플링 오프셋
	const int offset1 = 2;
	const int offset2 = 4;

	// 아직도 부들부들 떨림. 수정바람.

	left_y1 = GetTop(terrain, mapSize, p_x - offset1, left_y1);
	left_y2 = GetTop(terrain, mapSize, p_x - offset2, left_y2);
	right_y1 = GetTop(terrain, mapSize, p_x + offset1, right_y1);
	right_y2 = GetTop(terrain, mapSize, p_x + offset2, right_y2);

	float left_angle1 = atan((float)(left_y1 - left_y2) / (offset2 - offset1));
	float left_angle2 = atan((float)(center_y - left_y1) / (offset1));

	float right_angle1 = atan((float)(right_y2 - right_y1) / (offset2 - offset1));
	float right_angle2 = atan((float)(right_y1 - center_y) / (offset1));

	float ret = (((left_angle1 + left_angle2)/2 + (right_angle1 + right_angle2)/2)/2);

	return ret;
}

Point Tank::GetWeaponPosition() const
{
	return weapon[selectedWeapon]->GetPosition();
}

// 존나 더럽다 코드 분석 시 안구 주의 요망
bool Tank::Update(float dt)
{
	if(!isDead && (y <= -50 || life <= 0))
	{
		sprite.SetAnimation("Dead");
		onDestorySig(this);
		isDead = true;
	}

	Terrain terrain = map->GetTerrain();
	Size mapSize = map->GetMapSize();

	bool busy = false;

	// 이동 가능한가?
	bool isMovable = remainMovingTime > 0.0f;

	int bottom_min = GetTop(terrain, mapSize, x, y);
	if(bottom_min <= 0) bottom_min = -100;

	// 떨어진다!
	if(bottom_min + 15 < y)
	{
		verticalSpeed += 9.8f * 100.0f * dt;

		float bottom = y - 0.5f * verticalSpeed * dt;

		if(bottom > bottom_min)
		{
			busy |= true;
			isMovable = false;
		}
		else
		{
			bottom = bottom_min;
		}
		y = bottom;
	}
	else
	{
		verticalSpeed = 0.0f;
		y = GetTop(terrain, mapSize, x, y);

		angle = GetTankAngleOnPoint(x, y);

		sprite.SetAngle(angle * 180.0f / 3.141592f);
	}

	if(direction != Stopped && isMovable && !isDead)
	{
		// 다음 x 좌표 = 지금 x 좌표 + 방향 * dt * 속도(핵)
		float next_x = x + direction * dt * 80.0f;

		// 갈 수 있는 경로인지부터 확인한다.
		// probe가 지금 x 좌표에서부터 다음 x 좌표 직전까지 한 픽셀식 돌아가면서 조사하는데
		// probe 에서 probe + 1 까지 뚫려있는지 확인한다.
		// 한 번이라도 막혀있으면 못 가는 것
		for(int probe = (int)x; ((int)next_x -probe) * direction > 0 && isMovable; probe += direction)
		{
			int probe_y = GetTop(terrain, mapSize, probe, y);
			int side = GetTop(terrain, mapSize, probe + direction, y);

			if(side > probe_y + 5)
			{
				isMovable = false;
				break;
			}
			else
			{
				for(int i = probe_y + 1; i <= side; ++i)
				{
					if(terrain(probe, i))
					{
						isMovable = false;
						break;
					}
				}
			}
		}


		// 갈 수 있다면 경사진 곳을 감안하여 주어진 시간에 어디까지 갔어야 하는지 정한다.
		// 아 진짜 더럽다. 돌머리는 어쩔 수 없음
		if(isMovable)
		{
			// 누적된 x축 이동 편차량
			float minus = 0.0f;

			for(float probe = x; next_x - probe >= 0.0f; probe += direction)
			{
				// 일단 지금 높이와 바로 옆 픽셀의 높이의 차이를 구함.
				int side = GetTop(terrain, mapSize, probe + direction, y);
				side -= GetTop(terrain, mapSize, probe, y);

				// 평지거나 내리막이면 알고리즘 적용 안함
				if(side <= 0) continue;

				// 피타고라스의 공식을 이용해서 대각선 길이를 구한다.
				// distance는 전체 대각선 길이고, diff는 기울기에 따른 x축 방향의 편차.
				float distance = sqrt((float)(1 + side * side));
				float diff = (distance - 1.0f) / distance;

				// 1픽셀보다 더 많이 이동한 게 아니라면 편차에 비율을 적용한다.
				if(next_x - probe < 1.0f)
					diff *= (next_x - probe);

				// 편차 누적
				minus -= diff;
			}

			//편차 적용
			x = next_x + minus;

			remainMovingTime -= dt;
			remainMovingTime = (remainMovingTime < 0.0f)? 0.0f : remainMovingTime;
		}
	}

	sprite.SetPosition(Point(x, y));
	sprite.Update(dt);
	busy |= weapon[selectedWeapon]->Update(dt);
	busy &= !isDead;

	return busy;
}

boost::signals2::connection Tank::ConnectFireEvent(boost::function<void(Weapon* weapon)> fn)
{
	return onFireSig.connect(fn);
}

boost::signals2::connection Tank::ConnectDestoryEvent(boost::function<void(Tank* tank)> fn)
{
	return onDestorySig.connect(fn);
}
}

