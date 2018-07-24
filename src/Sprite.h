#pragma once

#include <json/json.h>

#include "Texture.h"
#include "Types.h"
#include "Drawable.h"
#include "Renderer.h"

namespace PangPang
{
class Sprite : public Drawable
{
public:
	Sprite(Renderer* renderer, Renderer::LayerType layerType);
	Sprite(Renderer* renderer, Renderer::LayerType layerType, const char *path);
	~Sprite(void);
public:
	void Load(const char *path);

	void SetAnimation(const char *animationName);
	void SetAnimation(std::string animationName);

	std::vector<std::string> GetAnimationList();

	void Update(float deltaTime);

	std::shared_ptr<Texture> GetTexture() {
		return texture;
	}

	float GetAngle() const;
	void SetAngle(float val);
	Point GetAnchorPoint() const;
	void SetAnchorPoint(Point p);
	Point GetPosition() const;
	void SetPosition(Point p);
	Size GetSize() const;
	void SetSize(Size s);

	float GetAnimationSpeed() const;
	void SetAnimationSpeed(float val);
	float GetFrameInterval() const;
	void SetFrameInterval(float val);
	float GetAnimationTime() const;
	void SetAnimationTime(float val);

	Size GetFrameSize() const;
	void SetFrameSize(Size s);

	uint GetFrameCount() const;
	void SetFrameCount(uint val);

	bool GetHorizontalFlipped() const;
	void SetHorizontalFlipped(bool val);

	bool GetVisible() const;
	void SetVisible(bool val);

protected:
	virtual void Draw();

private:
	float angle;
	Point anchorPoint;
	Point position;
	Size size;

	float animationSpeed;
	float frameInterval;
	float animationTime;

	Size frameSize;
	uint frameCount;

	bool horizontalFlipped;
	bool isVisible;

	std::map<std::string, std::shared_ptr<Texture>> textureMap;
	std::shared_ptr<Texture> texture;
	Json::Value jsonFile;
};
}

