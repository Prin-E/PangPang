#pragma once

#include "UIElement.h"

namespace PangPang 
{
	namespace UI 
	{
		class Bar : public UIElement
		{
		public:
			Bar();
			~Bar();

			void SetMinValue(int32 val);
			void SetMinValue(float val);
			float GetMinValue() const;

			void SetMaxValue(int32 val);
			void SetMaxValue(float val);
			float GetMaxValue() const;

			void SetValue(int32 val);
			void SetValue(float val);
			float GetValue() const;

			void SetColor(float r, float g, float b);
			void SetBorderWidth(float w);
			float GetBorderWidth() const;

			virtual void Draw();

		private:
			float r, g, b;
			float borderWidth;

			float minValue, maxValue;
			float value;
		};
	}
}