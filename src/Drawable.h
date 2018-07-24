#pragma once

#include "Renderer.h"

namespace PangPang
{
class Drawable
{
public:
	Drawable(Renderer* renderer, Renderer::LayerType layerType)
		:	renderer(renderer), layerType(layerType), isRegistered(false)
	{
		Register();
	}
	virtual ~Drawable()
	{
		Unregister();
	}
public:
	virtual void Draw() = 0;

	void Register()
	{
		if(!isRegistered)
		{
			renderer->Register(this, layerType);
			isRegistered = true;
		}
	}
	void Unregister()
	{
		if(isRegistered)
		{
			renderer->Unregister(this, layerType);
			isRegistered = false;
		}
	}
private:
	Renderer* renderer;
	Renderer::LayerType layerType;
	bool isRegistered;
};
}

