#include "Common.h"
#include "Types.h"
#include "Bar.h"

namespace PangPang 
{
	namespace UI
	{
		Bar::Bar() 
			: UIElement(Point(0, 0), Size(120, 15))
		{
			minValue = 0;
			maxValue = 100.0f;
			value = 50.0f;

			r = 0.15f; g = 0.25f; b = 0.80f;
			borderWidth = 1.0f;
		}

		Bar::~Bar() 
		{

		}

		void Bar::SetMinValue(int32 val)
		{
			if((float)val >= value)
				minValue = value;
			else
				minValue = (float)val;
		}

		void Bar::SetMinValue(float val)
		{
			if(val >= value)
				minValue = value;
			else
				minValue = val;
		}

		float Bar::GetMinValue() const
		{
			return minValue;
		}

		void Bar::SetMaxValue(int32 val)
		{
			if((float)val <= value)
				maxValue = value;
			else
				minValue = (float)val;
		}

		void Bar::SetMaxValue(float val)
		{
			if(val <= value)
				maxValue = value;
			else
				minValue = val;
		}

		float Bar::GetMaxValue() const
		{
			return maxValue;
		}

		void Bar::SetValue(int32 val)
		{
			if((float)val <= minValue)
				value = minValue;
			else if((float)val >= maxValue)
				value = maxValue;
			else
				value = (float)val;
		}

		void Bar::SetValue(float val)
		{
			if(val <= minValue)
				value = minValue;
			else if(val >= maxValue)
				value = maxValue;
			else
				value = val;
		}

		float Bar::GetValue() const
		{
			return value;
		}

		void Bar::SetColor(float r, float g, float b) 
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

		void Bar::SetBorderWidth(float w) 
		{
			if(w <= 0.0f) w = 0.0f;
			borderWidth = w;
		}

		float Bar::GetBorderWidth() const
		{
			return borderWidth;
		}

		void Bar::Draw()
		{
			Point p = this->GetPosition();
			Size s = this->GetSize();

			float percent = (value - minValue) / (maxValue - minValue);

			glLineWidth(borderWidth);

			// 안티 앨리어싱
			glEnable(GL_POLYGON_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			// 블렌딩
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// 그리기 시작
			glBegin(GL_QUADS);

			// 배경 그리기
			glColor4f(r * 0.5f, g * 0.5f, b * 0.5f, 0.6f);
			glVertex2i(p.x, p.y);
			glVertex2i(p.x+s.width, p.y);
			glVertex2i(p.x+s.width, p.y+s.height);
			glVertex2i(p.x, p.y+s.height);

			// 진행 상황 막대 그리기
			glColor4f(r, g, b, 0.85f);
			glVertex2f((float)p.x, (float)p.y);
			glVertex2f(float(p.x)+percent*float(s.width), float(p.y));
			glVertex2f(float(p.x)+percent*float(s.width), float(p.y+s.height));
			glVertex2f(float(p.x), float(p.y+s.height));
			glEnd();

			// 경계선 그리기
			glBegin(GL_LINE_LOOP);
			glColor4f(r * 1.2f, g * 1.2f, b * 1.2f, 1.0f);
			glVertex2i(p.x, p.y);
			glVertex2i(p.x+s.width, p.y);
			glVertex2i(p.x+s.width, p.y+s.height);
			glVertex2i(p.x, p.y+s.height);
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnd();

			// 끝
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
			glDisable(GL_BLEND);
		}
	}
}