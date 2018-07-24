#include "Common.h"
#include "Button.h"

namespace PangPang 
{
	namespace UI 
	{
		Button::Button(Event::Emitter<Event::Mouse>* emitter) 
			: UIElement(Point(0, 0), Size(24, 24)), Listener(emitter),
			texture(new Texture()), frameSize(Size(24, 24))
		{
			state = None;
			label.SetTextAlignment(TextAlignmentMiddle);

			this->SetTextColor(Color(0, 0, 0, 1));
			this->SetTextSize(10);
			this->SetText(L"");
		}

		Button::Button(Event::Emitter<Event::Mouse>* emitter, std::shared_ptr<Texture> t)
			: UIElement(Point(0, 0), Size(24, 24)), Listener(emitter),
			texture(t), frameSize(Size(24, 24))
		{
			this->SetPosition(Point(0, 0));
			this->SetSize(Size(24, 24));
			state = None;
			label.SetTextAlignment(TextAlignmentMiddle);

			this->SetTextColor(Color(0, 0, 0, 1));
			this->SetTextSize(10);
			this->SetText(L"");
		}

		Button::~Button() 
		{
		}

		void Button::SetFrameSize(Size s)
		{
			frameSize = s;
		}

		Size Button::GetFrameSize() const
		{
			return frameSize;
		}

		void Button::SetTexture(std::shared_ptr<Texture> t)
		{
			texture = t;
		}

		void Button::SetTextureAtPath(std::string str) 
		{
			std::shared_ptr<Texture> t(new Texture(str.c_str()));
			texture = t;
		}

		std::shared_ptr<Texture> Button::GetTexture() const
		{
			return texture;
		}

		void Button::SetText(std::wstring text) {
			label.SetText(text);
		}

		std::wstring Button::GetText() const {
			return label.GetText();
		}

		void Button::SetTextSize(uint32 size) {
			Size s = this->GetSize();
			if(size > (uint32)s.height)
				size = s.height;

			textSize = size;
			label.SetTextSize(size);
		}

		uint32 Button::GetTextSize() const {
			return textSize;
		}

		void Button::SetTextColor(Color color) {
			textColor = color;
			Range r(0, label.GetTextLength());
			label.SetTextColorInRange(color, r);
		}

		Color Button::GetTextColor() const {
			return textColor;
		}

		boost::signals2::connection Button::RegisterHitEventCallback(boost::function<void(int)> fn)
		{
			return action.connect(fn);
		}

		void Button::Draw() 
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			texture->BindTexture();
			Size imageSize = texture->ImageSize();

			Point position = this->GetPosition();
			Size size = this->GetSize();

			float tw1, tw2, th1, th2;
			tw1 = 0.0f;
			tw2 = float(frameSize.width) / float(imageSize.width);

			th1 = float(imageSize.height - frameSize.height * (state+1)) / float(imageSize.height);
			th2 = float(imageSize.height - frameSize.height * (state)) / float(imageSize.height);

			glBegin(GL_QUADS);
			glTexCoord2f(tw1, th1);
			glVertex2i(position.x, position.y);
			glTexCoord2f(tw2, th1);
			glVertex2i(position.x + size.width, position.y);
			glTexCoord2f(tw2, th2);
			glVertex2i(position.x + size.width, position.y + size.height);
			glTexCoord2f(tw1, th2);
			glVertex2i(position.x, position.y + size.height);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			// 라벨 그리기
			if(textSize <= size.height)
			position.y += (size.height - textSize)  * 2 / 3;

			label.SetPosition(position);
			label.SetSize(size);
			label.Draw();
		}

		void Button::Handler(const Event::Mouse::List& list)
		{
			Point position = this->GetPosition();
			Size size = this->GetSize();

			for(auto it = list.begin(); it != list.end(); it++) 
			{
				const Event::Mouse& mouseEvent = (*it);
				Point p = mouseEvent.locationInWindow;

				switch(mouseEvent.type) {
				case Event::Mouse::Move:
					if(position.x <= p.x && position.y <= p.y &&
						position.x + size.width >= p.x && position.y + size.height >= p.y)
						state = Over;
					else
						state = None;
					break;
				case Event::Mouse::LeftDown:
					if(position.x <= p.x && position.y <= p.y &&
						position.x + size.width >= p.x && position.y + size.height >= p.y)
						state = Pressed;
					else
						state = None;
					break;
				case Event::Mouse::LeftDragged:
					if(position.x <= p.x && position.y <= p.y &&
						position.x + size.width >= p.x && position.y + size.height >= p.y)
						state = Pressed;
					else
						state = None;
					break;
				case Event::Mouse::LeftUp:
					if(position.x <= p.x && position.y <= p.y &&
						position.x + size.width >= p.x && position.y + size.height >= p.y)
					{
						action(this->GetTag());
					}
					state = None;
					break;
				}
			}
		}
	}
}
