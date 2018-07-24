#pragma once

#include "Texture.h"
#include "Types.h"
#include "Renderer.h"
#include "Event.h"
#include "Label.h"

#include "UIElement.h"

#include <boost/function.hpp>
#include <boost/signals2.hpp>

namespace PangPang 
{
	namespace UI 
	{
		class Button : public UIElement, public Event::Listener<Event::Mouse>
		{
		public:
			Button(Event::Emitter<Event::Mouse>* emitter);
			Button(Event::Emitter<Event::Mouse>* emitter, std::shared_ptr<Texture> t);
			~Button();

			void SetFrameSize(Size s);
			Size GetFrameSize() const;

			void SetTexture(std::shared_ptr<Texture> t);
			void SetTextureAtPath(std::string str);
			std::shared_ptr<Texture> GetTexture() const;

			void SetText(std::wstring text);
			std::wstring GetText() const;

			void SetTextSize(uint32 size);
			uint32 GetTextSize() const;

			void SetTextColor(Color color);
			Color GetTextColor() const;

			boost::signals2::connection RegisterHitEventCallback(boost::function<void(int)> fn);

			void Draw();

		public: // EventListener
			virtual void Handler(const Event::Mouse::List& list);

		private:
			enum ButtonState {
				None = 0,
				Over = 1,
				Pressed = 2
			};
			unsigned int state;
			
			boost::signals2::signal<void(int)> action;
			std::shared_ptr<Texture> texture;

			Size frameSize;

			// Text
			Label label;
			Color textColor;
			uint32 textSize;
		};
	}
}