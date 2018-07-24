#pragma once

#include "UIElement.h"
#include "Texture.h"
#include "Types.h"

namespace PangPang
{
	namespace UI
	{
		class ImageView : public UIElement
		{
		public:
			ImageView();
			~ImageView();

			void SetImageAtPath(const char *path);
			void SetImageWithTexture(std::shared_ptr<Texture> theTexture);
			std::shared_ptr<Texture> GetImage() const;

			void SetFrameRect(Rect r);
			Rect GetFrameRect() const;

			virtual void Draw();

		private:
			std::shared_ptr<Texture> image;
			Rect frameRect;
		};
	}
}

