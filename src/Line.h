#pragma once

#include "Drawable.h"
#include "Renderer.h"
#include "Types.h"

namespace PangPang
{
class Line : public Drawable
{
public:
	Line(Renderer *renderer, Renderer::LayerType layerType);
	~Line(void);

	void SetColor(float r, float g, float b);

	void SetBeginPoint(Point p);
	void MoveToPoint(Point p);
	void CloseLine();

	Point GetPointAtIndex(uint32 index);
	std::vector<Point> GetPoints();

	float GetLineWidth();
	void SetLineWidth(float val);

	virtual void Draw();

protected:
private:
	std::vector<Point> points;
	float lineWidth;
	float r, g, b;
};
}

