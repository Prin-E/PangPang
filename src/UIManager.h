#pragma once

#include "Drawable.h"
#include "UIElement.h"
#include "Types.h"

namespace PangPang
{
	namespace UI
	{
		class UIManager: public Drawable
		{
		public:
			UIManager(Renderer *renderer);
			~UIManager();

			std::list<std::shared_ptr<UIElement>> UIList() const;
			
			void AddUI(std::shared_ptr<UIElement> ui, uint32 zIndex);

			virtual void Draw();
		private:
			std::list<std::shared_ptr<UIElement>> uilist;
			std::list<uint32> zindexlist;
		};
	}
}
