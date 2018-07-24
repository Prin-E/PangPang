#include "Common.h"
#include "Types.h"
#include "AngleView.h"
#include <cmath>

const double c_pi = 3.14159;

namespace PangPang
{
	namespace UI
	{
		AngleView::AngleView()
			: UIElement(Point(0, 0), Size(80, 80))
		{
			minAngle = 0.0f;
			maxAngle = 45.0f;
			angle = 20.0f;

			borderWidth = 1.0f;
			lineWidth = 1.5f;
			r = 0.15f; g = 0.25f; b = 0.80f;
		}

		AngleView::~AngleView()
		{

		}

		void AngleView::SetMinAngle(int32 val)
		{
			minAngle = (float)val;
		}

		void AngleView::SetMinAngle(float val)
		{
			minAngle = val;
		}

		float AngleView::GetMinAngle() const
		{
			return minAngle;
		}

		void AngleView::SetMaxAngle(int32 val)
		{
			maxAngle = (float)val;
		}

		void AngleView::SetMaxAngle(float val)
		{
			maxAngle = val;
		}

		float AngleView::GetMaxAngle() const
		{
			return maxAngle;
		}

		void AngleView::SetAngle(int32 val)
		{
			angle = (float)val;
		}

		void AngleView::SetAngle(float val)
		{
			angle = val;
		}

		float AngleView::GetAngle() const 
		{
			return angle;
		}

		void AngleView::SetColor(float r, float g, float b)
		{
			if(r >= 1.0f) r = 1.0f;
			else if(r <= 0.0f) r = 0.0f;
			if(g >= 1.0f) g = 1.0f;
			else if(g <= 0.0f) g = 0.0f;
			if(b >= 1.0f) b = 1.0f;
			else if(b <= 0.0f) b = 0.0f;

			this->r = r;
			this->g = g;
			this->b = b;
		}

		void AngleView::SetBorderWidth(float val) 
		{
			if(val <= 0.0f) val = 0.0f;
			borderWidth = val;
		}

		float AngleView::GetBorderWidth() const
		{
			return borderWidth;
		}

		void AngleView::SetLineWidth(float val) 
		{
			if(val <= 0.0f) val = 0.0f;
			lineWidth = val;
		}

		float AngleView::GetLineWidth() const
		{
			return lineWidth;
		}

		void AngleView::Draw()
		{
			Point p = this->GetPosition();
			Size s = this->GetSize();

			float centerX, centerY;			// 원 중심
			centerX = float(p.x) + float(s.width) / 2.0f;
			centerY = float(p.y) + float(s.height) / 2.0f;

			float radius;					// 반지름
			if(s.width < s.height)			// 너비/높이 중 가장 작은 변의 1/2배를 반지름으로 한다.
				radius = float(s.width) / 2.0f;
			else
				radius = float(s.height) / 2.0f;

			// 안티 앨리어싱
			glEnable(GL_POLYGON_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			// 블렌딩
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			int numOfFan = 0;

			// 배경 그리기
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(r * 0.8f, g * 0.8f, b * 0.8f, 0.6f);
			glVertex2f(centerX, centerY);

			if(minAngle <= maxAngle) {
				numOfFan = int(360 + minAngle - maxAngle) + 1;
				for(float f = maxAngle; f <= 360.0f+minAngle; f += (360.0f + minAngle - maxAngle)/float(numOfFan))
					glVertex2f(centerX + radius * cosf(f / 180.0f * c_pi), 
					centerY + radius * sinf(f / 180.0f * c_pi));
			}
			else {
				numOfFan = int(minAngle - maxAngle) + 1;
				for(float f = maxAngle; f <= minAngle;  f += (minAngle - maxAngle)/float(numOfFan))
					glVertex2f(centerX + radius * cosf(f / 180.0f * c_pi), 
					centerY + radius * sinf(f / 180.0f * c_pi));
			}

			glEnd();

			// 최소/최대 사이의 각도 범위 그리기
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(r, g, b, 0.75f);
			glVertex2f(centerX, centerY);

			if(minAngle <= maxAngle) {
				numOfFan = int(maxAngle-minAngle) + 1;
				for(float f = minAngle; f <= maxAngle; f += (maxAngle-minAngle)/float(numOfFan))
					glVertex2f(centerX + radius * cosf(f / 180.0f * c_pi), 
							   centerY + radius * sinf(f / 180.0f * c_pi));
			}
			else {
				numOfFan = int(360+maxAngle-minAngle) + 1;
				for(float f = minAngle; f <= 360.0f+maxAngle; f += (360+maxAngle-minAngle)/float(numOfFan))
					glVertex2f(centerX + radius * cosf(f / 180.0f * c_pi), 
							   centerY + radius * sinf(f / 180.0f * c_pi));
			}
			glEnd();

			// 최소/최대/현재 각도 나타내는 직선 그리기
			glLineWidth(lineWidth);
			glBegin(GL_LINES);

			// 색상은 흰색 또는 검은색으로
			float lineColor = sqrtf(r*r+g*g+b*b);
			if(lineColor < 0.7f) lineColor = 1.0f;
			else lineColor = 0.0f;

			glColor4f(lineColor, lineColor, lineColor, 1.0f);

			// 최소 각도
			glVertex2f(centerX, centerY);
			glVertex2f(centerX + radius * cosf(minAngle / 180.0f * c_pi), 
					   centerY + radius * sinf(minAngle / 180.0f * c_pi));

			// 최대 각도
			glVertex2f(centerX, centerY);
			glVertex2f(centerX + radius * cosf(maxAngle / 180.0f * c_pi), 
				centerY + radius * sinf(maxAngle / 180.0f * c_pi));

			// 현재 각도
			glVertex2f(centerX, centerY);
			glVertex2f(centerX + radius * cosf(angle / 180.0f * c_pi), 
					   centerY + radius * sinf(angle / 180.0f * c_pi));
			glEnd();


			// 경계선 그리기
			glLineWidth(borderWidth);
			glBegin(GL_LINE_LOOP);
			glColor4f(r * 1.2f, g * 1.2f, b * 1.2f, 1.0f);
			for(float f = 0.0f; f < 360.0f; f += 1.0f)
				glVertex2f(centerX + radius * cosf(f / 180.0f * c_pi), 
						   centerY + radius * sinf(f / 180.0f * c_pi));
			glEnd();

			// 끝
			glColor3f(1.0f, 1.0f, 1.0f);
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
			glDisable(GL_BLEND);
		}
	}
}