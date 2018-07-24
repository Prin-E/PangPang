#include "Common.h"
#include "Types.h"
#include "UIElement.h"

namespace PangPang 
{
	namespace UI 
	{
		UIElement::UIElement(Point thePosition, Size theSize)
			: position(thePosition), size(theSize), tag(0)
		{
		}

		UIElement::~UIElement()
		{

		}

		void UIElement::SetPosition(Point p)
		{
			position = p;
		}

		Point UIElement::GetPosition() const
		{
			return position;
		}

		void UIElement::SetSize(Size s)
		{
			size = s;
		}

		Size UIElement::GetSize() const
		{
			return size;
		}

		void UIElement::SetTag(uint32 i)
		{
			tag = i;
		}

		uint32 UIElement::GetTag() const
		{
			return tag;
		}
	}
}