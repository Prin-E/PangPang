#pragma once

#include "UIElement.h"
#include "Renderer.h"
#include "Texture.h"
#include "Types.h"

// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_ERRORS_H
#include FT_GLYPH_H

namespace PangPang
{
	namespace UI
	{
		struct CharacterInfo {
			wchar_t character;
			uint32 size;

			bool operator ==(const CharacterInfo &c) const {
				if(this->character == c.character && 
					this->size == c.size)
					return true;
				else
					return false;
			}

			bool operator !=(const CharacterInfo &c) const {
				if(this->character == c.character && 
					this->size == c.size)
					return false;
				else
					return true;
			}

			bool operator <(const CharacterInfo &c) const {
				if(c.character == character)
					return size < c.size;
				else
					return character < c.character;
			}

			bool operator >(const CharacterInfo &c) const {
				if(c.character == character)
					return size > c.size;
				else
					return character > c.character;
			}

			bool operator <=(const CharacterInfo &c) const {
				if(c.character == character)
					return size <= c.size;
				else
					return character <= c.character;
			}

			bool operator >=(const CharacterInfo &c) const {
				if(c.character == character)
					return size >= c.size;
				else
					return character >= c.character;
			}
		};

		struct GlyphImageInfo {
			uint32 bearingX;
			uint32 bearingY;
			uint32 advanceX;
			uint32 advanceY;

			uint32 width;
			uint32 height;
		};

		struct GlyphImageData {
			std::shared_ptr<Texture> glyphImage;

			CharacterInfo characterInfo;
			GlyphImageInfo glyphImageInfo;
		};

		enum TextAlignment {
			TextAlignmentLeft = 0,
			TextAlignmentMiddle,
			TextAlignmentRight
		};

		class Label : public UIElement
		{
		public:
			Label();
			~Label();

			void SetText(std::wstring theText);
			std::wstring GetText() const;

			void SetTextColor(Color color);
			Color GetTextColor() const;
			void SetTextColorInRange(Color color, Range range);
			void SetTextSize(uint32 textSize);

			uint32 GetTextLength() const;

			void SetTextAlignment(TextAlignment theTextAlignment);
			TextAlignment GetTextAlignment() const;

			virtual void Draw();

		private:
			void RegenerateFontData();
			GlyphImageData GenerateGlyphImageData(CharacterInfo characterInfo);

			std::wstring text;

			static std::map<CharacterInfo, std::shared_ptr<Texture>> glyphMap;
			static std::map<std::shared_ptr<Texture>, GlyphImageInfo> glyphImageInfoMap;

			std::list<GlyphImageData> glyphDatas;
			std::list<Color> textColors;

			uint32 textSize;
			Color textColor;
			TextAlignment textAlignment;
		};
	}
}