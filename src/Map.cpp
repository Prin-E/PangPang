#include "Common.h"
#include "Map.h"

namespace PangPang
{
Map::Map(Renderer *renderer, Renderer::LayerType layerType, const char *backgroundPath, const char *landPath)
	: Drawable(renderer, layerType), alphaMask((unsigned char *)NULL, [](uchar* p)->void{delete[] p;})
{
	std::shared_ptr<Texture> b(new Texture), l(new Texture);
	b->SetImageInFile(backgroundPath);
	l->SetImageInFile(landPath);

	background = b;
	land = l;

	mapSize = land->ImageSize();
	mapFrame = Rect(0, 0, mapSize.width, mapSize.height);

	unsigned int imageFormat = land->ImageFormat();

	if(imageFormat == RGBAFormat || imageFormat == BGRAFormat) {
		alphaMask.reset(new uchar[mapSize.width * mapSize.height]);
		uchar *p = (uchar *)land->Lock();

		for(int i = 0; i < mapSize.height; i++)
			for(int j = 0; j < mapSize.width; j++)
				alphaMask.get()[i * mapSize.width + j] = p[i*(mapSize.width*4)+j*4+3];

		land->Unlock();
	}
	else {
		alphaMask.reset();
	}
}

Map::Map(Renderer *renderer, Renderer::LayerType layerType, std::shared_ptr<Texture> backgroundTexture, std::shared_ptr<Texture> landTexture) 
	: Drawable(renderer, layerType) {
		background = backgroundTexture;
		land = landTexture;
}

std::shared_ptr<Texture> Map::GetBackgroundTexture() {
	return background;
}

std::shared_ptr<Texture> Map::GetLandTexture() {
	return land;
}

void Map::DigHole(Circle circle) {
	// alphaMask 가 NULL이면 작업을 수행하지 않는다.
	if(!alphaMask) return;

	int xMin, xMax, yMin, yMax;
	xMin = xMax = yMin = yMax = 0;

	Size mapSize = land->ImageSize();

	xMin = circle.center.x - circle.radius;
	if(xMin < 0) xMin = 0;
	xMax = circle.center.x + circle.radius;
	if(xMax > mapSize.width) xMax = mapSize.width;
	yMin = circle.center.y - circle.radius;
	if(yMin < 0) yMin = 0;
	yMax = circle.center.y + circle.radius;
	if(yMax > mapSize.height) yMax = mapSize.height;

	unsigned char *p = (unsigned char *)land->Lock();
	if(p == 0) {
		land->Unlock();
		return;
	}
	float r = 0.0f;

	for(int i = yMin; i < yMax; i++) {
		for(int j = xMin; j < xMax; j++) {
			r = sqrtf(float((j-circle.center.x)*(j-circle.center.x)+(i-circle.center.y)*(i-circle.center.y)));
			if(r <= float(circle.radius)) {
				alphaMask.get()[i*mapSize.width+j] = 0;
				p[i*(mapSize.width*4)+j*4+3] = 0;
			}
		}
	}

	land->Unlock();
}

Size Map::GetMapSize() const {
	return mapSize;
}

Rect Map::GetMapFrame() const {
	return mapFrame;
}

void Map::Draw() {
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw background
	background->BindTexture();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y + mapFrame.size.height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y + mapFrame.size.height);
	glEnd();

	// draw land
	land->BindTexture();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y + mapFrame.size.height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y + mapFrame.size.height);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Map::DrawForCollisionTest()
{
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw land
	land->BindTexture();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(mapFrame.origin.x + mapFrame.size.width, mapFrame.origin.y + mapFrame.size.height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(mapFrame.origin.x, mapFrame.origin.y + mapFrame.size.height);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
}

Rect Map::GetBoundingRect()
{
	Rect rect;
	rect.size = GetMapSize();
	return rect;
}

Map::~Map() {
}
}

