#include "Common.h"
#include "Renderer.h"
#include "Drawable.h"

namespace PangPang
{
const Renderer::LayerType Renderer::LayerOrder[Renderer::NumberOfLayers] = 
{
	BackgroundLayer,
	WeaponLayer,
	TankLayer,
	UiLayer
};

Renderer::Renderer(Window* window)
	:	layers(NumberOfLayers), window(window), viewPointX(0.0f), viewPointY(0.0f)
	,viewSize(800, 600), sceneOffset(Point(0, 150)), viewPointAnimationNeeded(false),
	viewPointDistX(0.0f), viewPointDistY(0.0f), viewPointDestX(0.0f), viewPointDestY(0.0f)
{

}

Renderer::Renderer(Window* window, Size aViewSize)
	:	layers(NumberOfLayers), window(window), viewPointX(0.0f), viewPointY(0.0f)
	,viewSize(aViewSize), sceneOffset(Point(0, 150)), viewPointAnimationNeeded(false),
	viewPointDistX(0.0f), viewPointDistY(0.0f), viewPointDestX(0.0f), viewPointDestY(0.0f)
{

}

void Renderer::SetViewPoint(float x, float y)
{
	Size contentSize = window->GetContentSize();

	if(x < 0) x = 0;
	else if(x + contentSize.width > viewSize.width)
		x = viewSize.width - contentSize.width;
	if(y < 0) y = 0;
	else if(y + contentSize.height > viewSize.height)
		y = viewSize.height - contentSize.height;

	viewPointX = x;
	viewPointY = y;
}

void Renderer::SetViewPoint(float x, float y, float animationTime)
{
	Size contentSize = window->GetContentSize();

	if(x < 0) x = 0;
	else if(x + contentSize.width > viewSize.width)
		x = viewSize.width - contentSize.width;
	if(y < 0) y = 0;
	else if(y + contentSize.height > viewSize.height)
		y = viewSize.height - contentSize.height;

	viewPointDestX = x;
	viewPointDestY = y;
	viewPointDistX = viewPointDestX - viewPointX;
	viewPointDistY = viewPointDestY - viewPointY;
	viewPointAnimationNeeded = true;
	animationElapsedTime = 0.0f;
	this->animationTime = animationTime;
}

void Renderer::SetViewPointWithCenterPoint(float x, float y)
{
	Size contentSize = window->GetContentSize();
	contentSize.width -= sceneOffset.x;
	contentSize.height -= sceneOffset.y;

	if(x + contentSize.width / 2 > viewSize.width)
		x = float(viewSize.width - contentSize.width / 2);
	else if(x - contentSize.width / 2 < 0)
		x = float(contentSize.width / 2);
	if(y + contentSize.height / 2 > viewSize.height)
		y = float(viewSize.height - contentSize.height / 2);
	else if(y - contentSize.height / 2 < 0)
		y = float(contentSize.height / 2);

	viewPointX = x - contentSize.width / 2;
	viewPointY = y - contentSize.height / 2;
}

void Renderer::SetViewPointWithCenterPoint(float x, float y, float animationTime)
{
	Size contentSize = window->GetContentSize();
	contentSize.width -= sceneOffset.x;
	contentSize.height -= sceneOffset.y;

	if(x + contentSize.width / 2 > viewSize.width)
		x = float(viewSize.width - contentSize.width / 2);
	else if(x - contentSize.width / 2 < 0)
		x = float(contentSize.width / 2);
	if(y + contentSize.height / 2 > viewSize.height)
		y = float(viewSize.height - contentSize.height / 2);
	else if(y - contentSize.height / 2 < 0)
		y = float(contentSize.height / 2);

	viewPointDestX = x - contentSize.width / 2;
	viewPointDestY = y - contentSize.height / 2;
	viewPointDistX = viewPointDestX - viewPointX;
	viewPointDistY = viewPointDestY - viewPointY;
	viewPointAnimationNeeded = true;
	animationElapsedTime = 0.0f;
	this->animationTime = animationTime;
}

void Renderer::SetSceneOffset(Point theOffset)
{
	Size contentSize = window->GetContentSize();

	if(theOffset.x > contentSize.width)
		theOffset.x = contentSize.width - 1;
	else if(theOffset.x < 0)
		theOffset.x = 0;
	if(theOffset.y > contentSize.height)
		theOffset.y = contentSize.height - 1;
	else if(theOffset.y < 0)
		theOffset.y = 0;

	sceneOffset = theOffset;
}

Point Renderer::GetSceneOffset() const
{
	return sceneOffset;
}

void Renderer::SetViewSize(Size theSize)
{
	Size contentSize = window->GetContentSize();

	if(theSize.width < contentSize.width) theSize.width = contentSize.width;
	if(theSize.height < contentSize.height) theSize.height = contentSize.height;

	viewSize = theSize;
}

Size Renderer::GetViewSize() const 
{
	return viewSize;
}

void Renderer::Update(float deltaTime)
{
	// 애니메이션
	if(viewPointAnimationNeeded) {
		animationElapsedTime += deltaTime;

		if(animationElapsedTime > animationTime) {
			viewPointAnimationNeeded = false;
			viewPointX = viewPointDestX;
			viewPointY = viewPointDestY;
			return;
		}

		float dX = viewPointDistX * deltaTime / animationTime;
		float dY = viewPointDistY * deltaTime / animationTime;

		viewPointX += dX; viewPointY += dY;

		if((viewPointDistX > 0 && viewPointX >= viewPointDestX)
			|| (viewPointDistX < 0 && viewPointX < viewPointDestX))
			viewPointX = viewPointDestX;
		if((viewPointDistY > 0 && viewPointY >= viewPointDestY)
			|| (viewPointDistY < 0 && viewPointY < viewPointDestY))
			viewPointY = viewPointDestY;
	}
}

void Renderer::Draw()
{
	boost::mutex::scoped_lock(mtx);
	for(int i = 0; i < NumberOfLayers; ++i)
	{
		std::list<Drawable*>& list = layers[LayerOrder[i]];

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		if(i < UiLayer) {
			glTranslatef(sceneOffset.x, sceneOffset.y, 0);
			glTranslatef(-viewPointX, -viewPointY, 0);
		}

		for(auto it = list.begin(); it != list.end(); ++it)
		{
			(*it)->Draw();
		}

		glPopMatrix();
	}
	window->SwapBuffers();
}

void Renderer::Register(Drawable* drawable, LayerType layerType)
{
	boost::mutex::scoped_lock(mtx);

	std::list<Drawable*>& list = layers[layerType];

	list.push_back(drawable);
}

void Renderer::Unregister(Drawable* drawable, LayerType layerType)
{
	boost::mutex::scoped_lock(mtx);

	std::list<Drawable*>& list = layers[layerType];

	list.remove(drawable);
}
}

