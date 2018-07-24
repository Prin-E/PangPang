#pragma once
#include "Types.h"

namespace PangPang 
{
	namespace UI 
	{
		class UIElement
		{
		public:
			UIElement(Point thePosition, Size theSize);
			~UIElement();

			void SetPosition(Point p);
			Point GetPosition() const;

			void SetSize(Size s);
			Size GetSize() const;

			void SetTag(uint32 i);
			uint32 GetTag() const;

			virtual void Draw() = 0;

		private:
			Point position;
			Size size;

			uint32 tag;
		};
	}
}