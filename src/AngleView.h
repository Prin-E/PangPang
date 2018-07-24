#pragma once

#include "UIElement.h"
#include "Drawable.h"
#include "Event.h"

namespace PangPang
{
	namespace UI
	{
		class AngleView: public UIElement
		{
		public:
			AngleView();
			~AngleView();

			void SetMinAngle(int32 val);
			void SetMinAngle(float val);
			float GetMinAngle() const;

			void SetMaxAngle(int32 val);
			void SetMaxAngle(float val);
			float GetMaxAngle() const;

			void SetAngle(int32 val);
			void SetAngle(float val);
			float GetAngle() const;

			void SetColor(float r, float g, float b);
			void SetBorderWidth(float w);
			float GetBorderWidth() const;
			void SetLineWidth(float w);
			float GetLineWidth() const;

			virtual void Draw();

		private:
			float minAngle, maxAngle;
			float angle;
			
			float r, g, b;
			float borderWidth;
			float lineWidth;
		};
	}
}