#include "Common.h"
#include "Types.h"
#include "Label.h"
#include "Resource.h"

namespace PangPang 
{
	static FT_Library library;
	static FT_Face face;

	static ResourcePtr r;

	const uint32 defaultTextSize = 20;	// 단위는 Pixel
	const Color defaultTextColor = Color(0, 0, 0, 1);

	inline int next_p2 (int a )
	{
		int rval=1;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2; 
		while(rval<a) rval<<=1;
		return rval;
	}

	static void InitFreeType() {
		FT_Error error;
		error = FT_Init_FreeType(&library);

		if(error)
		{
			std::fprintf(stderr, "FreeType 초기화 실패!\n");
			return;
		}


		r = Resource::Open("dir/NanumGothic.ttf");
		if(!r)
		{
			std::fprintf(stderr, "NanumGothic.ttf 를 찾을 수 없습니다!\n");
			return;
		}

		error = FT_New_Memory_Face(library, (const FT_Byte *)r->ptr, r->size, 0, &face);
		if(error)
		{
			std::fprintf(stderr, "FreeType Face 초기화 실패!\n");
			return;
		}
	}

	namespace UI
	{
		std::map<CharacterInfo, std::shared_ptr<Texture>> Label::glyphMap;
		std::map<std::shared_ptr<Texture>, GlyphImageInfo> Label::glyphImageInfoMap;

		Label::Label() 
			: UIElement(Point(0, 0), Size(100, 20))
		{
			static std::once_flag once_flag;
			std::call_once(once_flag, InitFreeType);
			//static boost::once_flag once_flag = BOOST_ONCE_INIT;
			//boost::call_once(once_flag, InitFreeType);

			textSize = defaultTextSize;
			textAlignment = TextAlignmentLeft;
			textColor = defaultTextColor;
		}

		Label::~Label()
		{
		}

		void Label::SetText(std::wstring theText)
		{
			if(theText == text)
				return;
			else
			{
				text = theText;
				this->RegenerateFontData();
			}
		}

		std::wstring Label::GetText() const
		{
			return text;
		}

		void Label::SetTextColor(Color color)
		{
			textColor = color;
		}

		Color Label::GetTextColor() const
		{
			return textColor;
		}

		void Label::SetTextColorInRange(Color color, Range range)
		{
			uint32 length = text.length();

			if(range.origin > length) return;
			else if(range.origin + range.length > length)
				range.length = length - range.origin;

			auto it = textColors.begin();

			uint32 i = 0;
			for(; i < length;)
			{
				if(i >= range.origin) break;
				i++; it++;
			}
			for(i = 0; i < range.length; i++, it++)
			{
				(*it) = color;
			}
		}

		void Label::SetTextSize(uint32 textSize)
		{
			if(textSize != this->textSize) {
				this->textSize = textSize;
				this->RegenerateFontData();
			}
		}

		uint32 Label::GetTextLength() const
		{
			return text.length();
		}

		void Label::SetTextAlignment(TextAlignment theTextAlignment)
		{
			if(theTextAlignment < TextAlignmentLeft || theTextAlignment > TextAlignmentRight)
				theTextAlignment = TextAlignmentLeft;
			textAlignment = theTextAlignment;
		}

		TextAlignment Label::GetTextAlignment() const
		{
			return textAlignment;
		}

		void Label::RegenerateFontData()
		{
			uint32 length = text.length();
			CharacterInfo ci;
			
			auto gdit = glyphDatas.begin();
			auto cit = textColors.begin();
			
			for(uint32 i = 0; i < length; i++)
			{
				ci.character = text[i];
				ci.size = textSize;

				if(gdit == glyphDatas.end())
				{
					GlyphImageData g = this->GenerateGlyphImageData(ci);
					glyphDatas.push_back(g);

					textColors.push_back(textColor);
				}
				else {
					if((*gdit).characterInfo != ci) {
						GlyphImageData g = this->GenerateGlyphImageData(ci);
						(*gdit) = g;
						(*cit) = textColor;
					}
					gdit++;
					cit++;
				}
			}

			// 잉여 데이터가 있으면 지워준다.
			if(gdit != glyphDatas.end())
			{
				glyphDatas.erase(gdit, glyphDatas.end());
				textColors.erase(cit, textColors.end());
			}
		}

		GlyphImageData Label::GenerateGlyphImageData(CharacterInfo characterInfo)
		{
			auto it = glyphMap.find(characterInfo);
			GlyphImageData g;

			if(it != glyphMap.end())
			{
				g.glyphImage = it->second;
				g.glyphImageInfo = glyphImageInfoMap[it->second];
				g.characterInfo = characterInfo;
			}
			else 
			{
				FT_Error error;

				FT_Set_Pixel_Sizes(face, 0, (uint32)textSize);

				uint32 index = FT_Get_Char_Index(face, characterInfo.character);
				error = FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
				if(error)
					std::fprintf(stderr, "Glyph 로딩 중 에러 발생!\n");
					
				FT_Glyph glyph;
				error = FT_Get_Glyph(face->glyph, &glyph);
				if(error)
					std::fprintf(stderr, "에러 : FT_Get_Glyph에서 발생\n");

				FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
				FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
				
				FT_Bitmap &bitmap = bitmap_glyph->bitmap;

				int width = next_p2(bitmap.width);
				int height = next_p2(bitmap.rows);

				GLubyte *expanded_data = new GLubyte[2 * width * height];

				// 비트맵 데이터를 expanded_data 에 복사
				for(int j=0; j <height;j++) {
					for(int i=0; i < width; i++){
						expanded_data[2*(i+j*width)] = 255;
						expanded_data[2*(i+j*width)+1] = 
							(i>=bitmap.width || j>=bitmap.rows) ?
							0 : bitmap.buffer[i + bitmap.width*j];
					}
				}

				std::shared_ptr<Texture> t(new Texture(expanded_data, Size(width, height), 2));

				GlyphImageInfo imageInfo;
				imageInfo.bearingX = bitmap_glyph->left;
				imageInfo.bearingY = bitmap_glyph->top-bitmap.rows;
				imageInfo.advanceX = face->glyph->advance.x >> 6;
				imageInfo.advanceY = glyph->advance.y >> 6;
				imageInfo.width = bitmap.width;
				imageInfo.height = bitmap.rows;

				glyphMap[characterInfo] = t;
				glyphImageInfoMap[t] = imageInfo;

				g.glyphImage = t;
				g.glyphImageInfo = imageInfo;
				g.characterInfo = characterInfo;
			}

			return g;
		}

		void Label::Draw() 
		{
			Point p = this->GetPosition();
			Size s = this->GetSize();

			Size imageSize;
			float tx, ty;
			int32 originX = p.x;

			if(textAlignment != TextAlignmentLeft) {
				int32 textWidth = 0;

				for(auto it = glyphDatas.begin(); it != glyphDatas.end(); it++)
				{
					 textWidth += it->glyphImageInfo.advanceX;
				}
				if(textAlignment == TextAlignmentMiddle)
					originX = p.x - (textWidth - s.width) / 2;
				else if(textAlignment == TextAlignmentRight)
					originX = p.x + s.width - textWidth;
			}

			if(textSize > (uint32)s.height) return;

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			auto cit = textColors.begin();
			for(auto it = glyphDatas.begin(); it != glyphDatas.end(); it++, cit++)
			{
				it->glyphImage->BindTexture();
				imageSize = it->glyphImage->ImageSize();

				tx = float(it->glyphImageInfo.width) / float(imageSize.width);
				ty = float(it->glyphImageInfo.height) / float(imageSize.height);

				if(originX >= (uint32)p.x) {
					glBegin(GL_QUADS);
					glColor4f(cit->r, cit->g, cit->b, cit->a);
					glTexCoord2f(0, ty);
					glVertex2i(originX + it->glyphImageInfo.bearingX, p.y + it->glyphImageInfo.bearingY);
					glTexCoord2f(tx, ty);
					glVertex2i(originX + it->glyphImageInfo.width + it->glyphImageInfo.bearingX, p.y + it->glyphImageInfo.bearingY);
					glTexCoord2f(tx, 0);
					glVertex2i(originX + it->glyphImageInfo.width + it->glyphImageInfo.bearingX, p.y + it->glyphImageInfo.height + it->glyphImageInfo.bearingY);
					glTexCoord2f(0, 0);
					glVertex2i(originX + it->glyphImageInfo.bearingX, p.y + it->glyphImageInfo.height + it->glyphImageInfo.bearingY);
					glEnd();
				}

				originX += it->glyphImageInfo.advanceX;

				if(originX >= (uint32)p.x + (uint32)s.width)
					break;
			}

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
}
