#pragma once

#include <iostream>

#include <GL/glew.h>
#include "../../ExternalResources/stb_image/stb_image.h"

namespace Rendering
{
	class Texture
	{
	private:
		unsigned int m_textureID;
		int m_textureWidth;
		int m_textureHeight;

		std::pair<float, float> m_topLeftPosition;
		std::pair<float, float> m_bottomRightPosition;
	public:
		Texture();
		~Texture();

		void LoadTexture(const char* path, bool smooth = true);
		void LoadTexture();

		void Bind() const { glBindTexture(GL_TEXTURE_2D, m_textureID); };

		unsigned int GetTexture() const { return m_textureID; };
		unsigned int GetTextureWidth() const { return (unsigned int)m_textureWidth; };
		unsigned int GetTextureHeight() const { return (unsigned int)m_textureHeight; };

		std::pair<float, float> GetFirstUVPosition() { return m_topLeftPosition; };
		std::pair<float, float> GetSecondUVPosition() { return m_bottomRightPosition; };

		void UseSection(float x, float y, float width, float height);
	};
}