#pragma once

#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Rendering
{
	struct Character
	{
		float advance;
		float top;
		float left;
		float width;
		float height;
		unsigned int stride;
	};

	class Font
	{
	private:
		const unsigned int m_SIZE = 48;
		unsigned int m_width, m_height;
		unsigned int m_texture;

		std::unordered_map<char, Rendering::Character> m_charactersData;
	public:
		Font(FT_Library freetypeLib, const char* path);

		void BindTextureAtlas() const { glBindTexture(GL_TEXTURE_2D, m_texture); };

		unsigned int GetTextureAtlas() const { return m_texture; };
		Rendering::Character GetCharacter(char c) const;
		float GetTextureAtlasWidth() const { return (float)m_width; };
		float GetTextureAtlasHeight() const { return (float)m_height; };
		unsigned int GetFontSize() const { return m_SIZE; };
	};
}