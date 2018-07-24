#include "Common.h"
#include "Sprite.h"
#include "Resource.h"

namespace PangPang
{
// 생성자
Sprite::Sprite(Renderer* renderer, Renderer::LayerType layerType)
	: Drawable(renderer, layerType), angle(0.0f), anchorPoint(Point(0, 0)), position(Point(0, 0)), size(Size(64, 64)),
	animationSpeed(1.0f), frameInterval(0.125f), animationTime(0.0f),
	frameSize(Size(48, 48)), frameCount(1), horizontalFlipped(false), isVisible(true)
{
	jsonFile = Json::nullValue;

	this->SetAnimation("Default");
}

Sprite::Sprite(Renderer* renderer, Renderer::LayerType layerType, const char *path)
	: Drawable(renderer, layerType), angle(0.0f), anchorPoint(Point(0, 0)), position(Point(0, 0)), size(Size(64, 64)),
	animationSpeed(1.0f), frameInterval(0.125f), animationTime(0.0f),
	frameSize(Size(48, 48)), frameCount(1), horizontalFlipped(false), isVisible(true) {
		this->Load(path);
}

void Sprite::Load(const char *path) {
	// 초기화
	textureMap.erase(textureMap.begin(), textureMap.end());

	// 파일 불러오기
	ResourcePtr r = Resource::Open(path);
	Json::Reader jsonReader;

	bool isRead = jsonReader.parse((const char *)r->ptr, (const char *)r->ptr+r->size, jsonFile);

	if(!isRead) {
		fprintf(stderr, "Json 파일 읽기 경고!\n");
		fprintf(stderr, "파일 문법이 잘못되었거나 존재하지 않습니다.\n\n");
	}

	this->SetAnimation("Default");
}

void Sprite::SetAnimation(const char *animationName) {
	Json::Value animationValue = jsonFile.get(animationName, Json::nullValue);

	// 값 설정
	angle = animationValue.get("angle", angle).asDouble();

	Json::Value anchorPointValue = animationValue.get("anchorPoint", Json::nullValue);

	anchorPoint.x = anchorPointValue.get("x", anchorPoint.x).asInt();
	anchorPoint.y = anchorPointValue.get("y", anchorPoint.y).asInt();
	this->SetAnchorPoint(anchorPoint);

	Json::Value sizeValue = animationValue.get("size", Json::nullValue);
	size.width = sizeValue.get("width", size.width).asInt();
	size.height = sizeValue.get("height", size.height).asInt();
	this->SetSize(size);

	animationSpeed = animationValue.get("animationSpeed", animationSpeed).asDouble();
	frameInterval = animationValue.get("frameInterval", frameInterval).asDouble();
	animationTime = animationValue.get("animationTime", animationTime).asDouble();

	Json::Value frameSizeValue = animationValue.get("frameSize", Json::nullValue);
	frameSize.width = frameSizeValue.get("width", Json::Value(48)).asInt();
	frameSize.height = frameSizeValue.get("height", Json::Value(48)).asInt();
	this->SetFrameSize(frameSize);

	frameCount = animationValue.get("frameCount", Json::Value(2)).asInt();

	std::string texturePath = animationValue.get("texture", Json::nullValue).asString();

	std::map<std::string, std::shared_ptr<Texture>>::iterator it = textureMap.find(std::string(animationName));
	if(it != textureMap.end()) {
		texture = it->second;
		return;
	}
	std::shared_ptr<Texture> p(new Texture);
	if(texturePath.length() > 0)
		p->SetImageInFile(texturePath.c_str());

	texture = p;
	textureMap[std::string(animationName)] = p;
}

void Sprite::SetAnimation(std::string animationName) {
	this->SetAnimation(animationName.c_str());
}

std::vector<std::string> Sprite::GetAnimationList() {
	const Json::Value::Members members = jsonFile.getMemberNames();

	std::vector<std::string> returnVector;
	for(int i = 0; i < members.size(); i++)
		returnVector.push_back(members[i]);

	return returnVector;
}

// 프로퍼티 메서드
float Sprite::GetAngle() const {
	return angle;
}

void Sprite::SetAngle(float val) {
	angle = val;
}

Point Sprite::GetAnchorPoint() const {
	return anchorPoint;
}

void Sprite::SetAnchorPoint(Point p) {
	anchorPoint = p;
}

Point Sprite::GetPosition() const {
	return position;
}

void Sprite::SetPosition(Point p) {
	position = p;
}

Size Sprite::GetSize() const {
	return size;
}

void Sprite::SetSize(Size s) {
	size = s;
}

float Sprite::GetAnimationSpeed() const {
	return animationSpeed;
}

void Sprite::SetAnimationSpeed(float val) {
	if(val < 0.00001f)
		val = 0.00001f;
	animationSpeed = val;
}

float Sprite::GetFrameInterval() const {
	return frameInterval;
}

void Sprite::SetFrameInterval(float val) {
	if(val < 0.00001f)
		val = 0.00001f;
	frameInterval = val;
}

float Sprite::GetAnimationTime() const {
	return animationTime;
}

void Sprite::SetAnimationTime(float val){
	if(val < 0.00001f)
		val = 0.00001f;
	animationTime = val;
}

Size Sprite::GetFrameSize() const {
	return frameSize;
}

void Sprite::SetFrameSize(Size s) {
	frameSize = s;
}

uint Sprite::GetFrameCount() const {
	return frameCount;
}

void Sprite::SetFrameCount(uint val) {
	frameCount = val;
}

bool Sprite::GetHorizontalFlipped() const {
	return horizontalFlipped;
}

void Sprite::SetHorizontalFlipped(bool val) {
	horizontalFlipped = val;
}

bool Sprite::GetVisible() const {
	return isVisible;
}

void Sprite::SetVisible(bool val) {
	isVisible = val;
}

// 업데이트
void Sprite::Update(float deltaTime) {
	animationTime += deltaTime;
}

// 그리기
void Sprite::Draw() {
	if(!isVisible)	// 숨기도록 설정했으면 그리지 않는다.
		return;

	if(texture) {
		glEnable(GL_TEXTURE_2D);
		texture->BindTexture();
	}
	int currentFrame = int(animationTime / frameInterval * animationSpeed);

	if(frameCount == 0) return;
	currentFrame %= frameCount;

	int widthCount, heightCount;
	Size imageSize = texture->ImageSize();

	if(frameSize.width == 0 || frameSize.height == 0) return;
	widthCount = imageSize.width / frameSize.width;
	heightCount = imageSize.height / frameSize.height;

	if(widthCount == 0) widthCount = 1;

	int wPos, hPos;
	hPos = currentFrame / widthCount;
	wPos = currentFrame % widthCount;

	float tw, th;
	tw = float(frameSize.width) / float(imageSize.width);
	th = float(frameSize.height) / float(imageSize.height);

	// Draw!
	texture->BindTexture();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// 회전
	glTranslatef(float(position.x), float(position.y), 0);
	glRotatef(angle, 0, 0, 1.0f);

	// 블렌딩
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 사각형 그리기
	float texCoordXMin = 0.0f, texCoordXMax = 1.0f, texCoordYMin = 0.0f, texCoordYMax = 1.0f;
	float theAnchorPointX = float(anchorPoint.x), theAnchorPointY = float(anchorPoint.y);

	if(horizontalFlipped == false) {
		texCoordXMin = float(wPos) * tw;
		texCoordXMax = float(wPos + 1) * tw;
	}
	else {
		texCoordXMin = float(wPos + 1) * tw;
		texCoordXMax = float(wPos) * tw;

		// anchorPoint X 값도 달라져야 한다(좌우가 바뀌므로 위치가 반전된다.)
		theAnchorPointX = float(size.width - anchorPoint.x);
	}

	texCoordYMin = 1.0f - float(hPos+1) * th;
	texCoordYMax = 1.0f - float(hPos) * th;

	glBegin(GL_QUADS);
	glTexCoord2f(texCoordXMin, texCoordYMin);
	glVertex2f(-theAnchorPointX, -theAnchorPointY);

	glTexCoord2f(texCoordXMax, texCoordYMin);
	glVertex2f(float(size.width)-theAnchorPointX, -theAnchorPointY);

	glTexCoord2f(texCoordXMax, texCoordYMax);
	glVertex2f(float(size.width)-theAnchorPointX, float(size.height)-theAnchorPointY);

	glTexCoord2f(texCoordXMin, texCoordYMax);
	glVertex2f(-theAnchorPointX, float(size.height)-theAnchorPointY);
	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

Sprite::~Sprite(void)
{
}
}

