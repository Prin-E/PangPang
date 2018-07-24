#include "Common.h"
#include "Line.h"

namespace PangPang
{
Line::Line(Renderer *renderer, Renderer::LayerType layerType)
	: Drawable(renderer, layerType)
{
	Point beginPoint = Point(0, 0);
	points.push_back(beginPoint);

	r = 1.0f;
	g = 1.0f;
	b = 1.0f;

	lineWidth = 1.0f;
}

void Line::SetColor(float r, float g, float b) {
	if(r < 0.0f) r = 0.0f;
	else if(r > 1.0f) r = 1.0f;
	if(g < 0.0f) g = 0.0f;
	else if(g > 1.0f) g = 1.0f;
	if(b < 0.0f) b = 0.0f;
	else if(g > 1.0f) b = 1.0f;

	this->r = r;
	this->g = g;
	this->b = b;
}

void Line::SetBeginPoint(Point p) {
	points.clear();

	points.push_back(p);
}

void Line::MoveToPoint(Point p) {
	points.push_back(p);
}

void Line::CloseLine() {
	points.push_back(points[0]);
}

Point Line::GetPointAtIndex(uint32 index) {
	return points[index];
}

std::vector<Point> Line::GetPoints() {
	return points;
}

float Line::GetLineWidth() {
	return lineWidth;
}

void Line::SetLineWidth(float val) {
	if(val < 0.1f) val = 0.1f;
	lineWidth = val;
}

void Line::Draw() {
	glLineWidth(lineWidth);

	glBegin(GL_LINE_STRIP);
	glColor3f(r, g, b);
	for(std::vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
		glVertex2i(it->x, it->y);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
}

Line::~Line(void)
{
}
}

