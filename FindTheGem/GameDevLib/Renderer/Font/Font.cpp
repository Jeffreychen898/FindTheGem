#include "Font.h"

namespace Rendering
{
	Font::Font(FT_Library freetypeLib, const char* path)
		: m_width(0), m_height(0), m_texture(0)
	{
		/* load in the font */
		FT_Face font;
		if (FT_New_Face(freetypeLib, path, 0, &font))
			std::cout << "[Err] Fail to load font at " << path << "!\n";

		FT_Set_Pixel_Sizes(font, 0, m_SIZE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* find the width and height needed for the texture */
		const unsigned int character_padding = 4;
		for (unsigned int i = 0; i < 128; ++i)
		{
			if (FT_Load_Char(font, i, FT_LOAD_RENDER))
			{
				std::cout << "[ERR] Fail to load character " << (char)i << " from " << path << "!\n";
				continue;
			}
			m_width += font->glyph->bitmap.width + character_padding * 2;
			m_height = std::max(m_height, font->glyph->bitmap.rows + character_padding * 2);
		}

		/* generate the texture */
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		unsigned int stride_x = 0;
		for (unsigned char i = 0; i < 128; ++i)
		{
			if (FT_Load_Char(font, i, FT_LOAD_RENDER))
				continue;

			/* store each characters in the texture */
			glTexSubImage2D(GL_TEXTURE_2D, 0, stride_x + character_padding, character_padding, font->glyph->bitmap.width,
				font->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);

			Rendering::Character character_data = {
				(float)(font->glyph->advance.x >> 6),
				(float)font->glyph->bitmap_top,
				(float)font->glyph->bitmap_left,
				(float)font->glyph->bitmap.width,
				(float)font->glyph->bitmap.rows,
				stride_x
			};

			m_charactersData.insert({ (char)i, character_data });

			stride_x += (unsigned int)character_data.width + character_padding * 2;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//subject to change
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//subject to change

		FT_Done_Face(font);
	}

	Rendering::Character Font::GetCharacter(char c) const
	{
		if (m_charactersData.find(c) == m_charactersData.end())
			return m_charactersData.at(' ');
		else
			return m_charactersData.at(c);
	}
}