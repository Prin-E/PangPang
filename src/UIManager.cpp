#include "Common.h"
#include "UIManager.h"

namespace PangPang
{
	namespace UI
	{
		UIManager::UIManager(Renderer *renderer)
			: Drawable(renderer, Renderer::UiLayer)
		{

		}

		UIManager::~UIManager()
		{

		}

		std::list<std::shared_ptr<UIElement>> UIManager::UIList() const
		{
			return uilist;
		}

		void UIManager::AddUI(std::shared_ptr<UIElement> ui, uint32 zIndex)
		{
			auto z_it = zindexlist.begin();
			auto ui_it = uilist.begin();

			while(z_it != zindexlist.end())
			{
				if((*z_it) > zIndex)
					break;

				z_it++;
				ui_it++;
			}
			uilist.insert(ui_it, ui);
			zindexlist.insert(z_it, zIndex);
		}

		void UIManager::Draw()
		{
			for(auto it = uilist.begin(); it != uilist.end(); it++)
			{
				(*it)->Draw();
			}
		}
	}
}