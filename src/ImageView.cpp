#include "Common.h"
#include "ImageView.h"

namespace PangPang
{
	namespace UI
	{
		ImageView::ImageView()
			: UIElement(Point(0, 0), Size(128, 96)), image(new Texture())
		{
		}

		ImageView::~ImageView()
		{

		}

		void ImageView::SetImageAtPath(const char *path)
		{
			std::shared_ptr<Texture> t(new Texture(path));

			image = t;
			frameRect.origin = Point(0, 0);
			frameRect.size = image->ImageSize();
		}

		std::shared_ptr<Texture> ImageView::GetImage() const
		{
			return image;
		}

		void ImageView::SetFrameRect(Rect r)
		{
			frameRect = r;
		}

		Rect ImageView::GetFrameRect() const
		{
			return frameRect;
		}

		void ImageView::Draw()
		{
			glEnable(GL_TEXTURE_2D);

			image->BindTexture();

			float tx0, tx1, ty0, ty1;
			Size imageSize = image->ImageSize();

			Point p = this->GetPosition();
			Size s = this->GetSize();

			tx0 = float(frameRect.origin.x) / float(imageSize.width);
			ty0 = float(frameRect.origin.y) / float(imageSize.height);

			tx1 = float(frameRect.origin.x + frameRect.size.width) / float(imageSize.width);
			ty1 = float(frameRect.origin.y + frameRect.size.height) / float(imageSize.height);

			glBegin(GL_QUADS);

			glTexCoord2f(tx0, ty0);
			glVertex2i(p.x, p.y);
			glTexCoord2f(tx1, ty0);
			glVertex2i(p.x+s.width, p.y);
			glTexCoord2f(tx1, ty1);
			glVertex2i(p.x+s.width, p.y+s.height);
			glTexCoord2f(tx0, ty1);
			glVertex2i(p.x, p.y+s.height);
			
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}
