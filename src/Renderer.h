#pragma once

#include <boost/thread/mutex.hpp>

#ifdef MULTITHREADED_RENDERER
#define RENDERER_MUTEX boost::mutex
#else
#include <boost/signals2/dummy_mutex.hpp>
#define RENDERER_MUTEX boost::signals2::dummy_mutex
#endif

#include "Window.h"

namespace PangPang
{
class Drawable;

class Renderer
{
	friend class Drawable;
public:
	enum LayerType
	{
		BackgroundLayer = 0,
		TankLayer,
		WeaponLayer,
		UiLayer,
		NumberOfLayers
	};
public:
	Renderer(Window* window);
	Renderer(Window* window, Size viewSize);
public:
	void Update(float deltaTime);
	void Draw();
public:
	void SetViewPoint(float x, float y);
	void SetViewPoint(float x, float y, float animationTime);
	void SetViewPointWithCenterPoint(float x, float y);
	void SetViewPointWithCenterPoint(float x, float y, float animationTime);
	float GetViewPointX() const { return viewPointX; }
	float GetViewPointY() const { return viewPointY; }

	void SetSceneOffset(Point theOffset);
	Point GetSceneOffset() const;

	void SetViewSize(Size theSize);
	Size GetViewSize() const;
private:
	void Register(Drawable* drawable, LayerType layerType);
	void Unregister(Drawable* drawable, LayerType layerType);
private:
	std::vector<std::list<Drawable*> > layers;
	static const LayerType LayerOrder[NumberOfLayers];

	RENDERER_MUTEX mtx;
	Window* window;

	// View 영역 관련
	float viewPointX, viewPointY;
	Point sceneOffset;
	Size viewSize;

	// View 영역 애니메이션
	bool viewPointAnimationNeeded;
	float viewPointDistX, viewPointDistY;
	float viewPointDestX, viewPointDestY;
	float animationElapsedTime;
	float animationTime;
};

}

