#pragma once

#include "Renderer.h"
#include "Drawable.h"
#include "Texture.h"
#include "Types.h"
#include "Collidable.h"

namespace PangPang
{
class Map;

class Terrain
{
	friend class Map;
private:
	Terrain(const std::shared_ptr<const uchar> p, Size size)
		:	holder(p),
		raw(p.get()),
		size(size)
	{

	}
public:
	uchar operator()(Point pt) const
	{
		if(pt.x < 0) pt.x = 0;
		if(pt.y < 0) pt.y = 0;
		if(size.width - 1 < pt.x) pt.x = size.width - 1;
		if(size.height - 1 < pt.y) pt.y = size.height - 1;
		return raw[pt.x + pt.y * size.width];
	}
	uchar operator()(int x, int y) const
	{
		if(x < 0) x = 0;
		if(y < 0) y = 0;
		if(size.width - 1 < x) x = size.width - 1;
		if(size.height - 1 < y) y = size.height - 1;
		return raw[x + y * size.width];
	}
private:
	std::shared_ptr<const uchar> holder;
	const uchar* raw;
	Size size;
};

class Map : public Drawable, public Collidable
{
public:
	Map(Renderer *renderer, Renderer::LayerType layerType, const char *backgroundPath, const char *landPath);
	Map(Renderer *renderer, Renderer::LayerType layerType, std::shared_ptr<Texture> backgroundTexture, std::shared_ptr<Texture> landTexture);
	~Map();

	std::shared_ptr<Texture> GetBackgroundTexture();
	std::shared_ptr<Texture> GetLandTexture();

	void DigHole(Circle circle);

	Size GetMapSize() const;
	Rect GetMapFrame() const;

	Terrain GetTerrain() const
	{
		return Terrain(alphaMask, mapSize);
	}

	void Draw();
private:
	void DrawForCollisionTest();
	Rect GetBoundingRect();
private:
	std::shared_ptr<Texture> background;
	std::shared_ptr<Texture> land;

	std::shared_ptr<uchar> alphaMask;

	Size mapSize;
	Rect mapFrame;
};
}

